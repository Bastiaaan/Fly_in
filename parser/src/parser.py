
from .connection import Connection
from .customError import ParseError, ParsingError
from .hub import Hub
from .map import Map
from json import dumps
from typing import Any
from pathlib import Path


class Parser:
    """The object who does the reading of the /maps-folder"""

    __target: str = "../result/map.json"
    __target_invalid: str = "../result/fail.json"

    def __init__(self) -> None:
        self._difficulty: str = "to be decided"
        self._map: str = "to be selected from"

    @staticmethod
    def check_directory() -> None:
        directory: Path = Path("result")
        if not directory.exists():
            directory.mkdir()

    @staticmethod
    def parse_arg(values: list[str], caster: Any) -> Any:
        if not values:
            return ""
        try:
            parsed: Any = caster(values[0])
        except (ValueError, KeyError, TypeError):
            return ""
        values.pop(0)
        return parsed

    @staticmethod
    def parse_map(path: str) -> Map | None:
        try:
            errors: list[ParseError] = []
            saved_hubs: list[Hub] = []
            saved_connections: list[Connection] = []
            nb_drones: int = 0
            line_buffer: str
            start: int = 0
            end: int = 0
            line_rule: int = 0
            with open(path, 'r') as _fstream:
                while True:
                    line_rule += 1
                    line_buffer = _fstream.readline()
                    args: dict[str, Any] = {}
                    if not line_buffer:
                        break
                    if '#' not in line_buffer[0] and not line_buffer == '\n':
                        key_row: list[str] = line_buffer.split(':')
                        if not len(key_row) == 2:
                            errors.append(
                                ParseError(
                                    line_rule,
                                    "declaring line with more than one ':'"))
                            continue
                        else:
                            key: str = key_row[0]
                            row: str = key_row[1]
                            row = row.strip().strip('\n')
                            if ("hub" in key or
                                "start_hub" in key or
                               "end_hub" in key):
                                values: list[str] = row.split(' ')
                                args["name"] = Parser.parse_arg(values, str)
                                args["x"] = Parser.parse_arg(values, int)
                                args["y"] = Parser.parse_arg(values, int)
                                args["color"] = None
                                args["max_drones"] = None
                                args["zone"] = None
                                args["position"] = None
                                if "start_hub" in key:
                                    start += 1
                                    args["position"] = "start"
                                    if start > 2:
                                        errors.append(
                                            ParseError(
                                                line_rule,
                                                "multi start_hub found"
                                            )
                                        )
                                        continue
                                elif "end_hub" in key:
                                    end += 1
                                    args["position"] = "end"
                                    if end > 2:
                                        errors.append(
                                            ParseError(
                                                line_rule,
                                                "multi end_hub found"
                                            )
                                        )
                                        continue
                                else:
                                    args["position"] = "midway"
                                if '[' in row and ']' in row:
                                    for value in values:
                                        value = value.strip().strip("[]")
                                        metaset: list[str] = value.split('=')
                                        if not len(metaset) == 2:
                                            errors.append(
                                                ParseError(
                                                    line_rule,
                                                    "incorrect assignment"))
                                        _key: str = metaset[0]
                                        _val: str = metaset[1]
                                        if _key in args:
                                            args[_key] = _val
                                hub: Hub = Hub(
                                    name=args["name"],
                                    x=args["x"],
                                    y=args["y"],
                                    color=args["color"],
                                    max_drones=args["max_drones"],
                                    zone=args["zone"],
                                    position=args["position"]
                                )
                                args.clear()
                                if any(_h.name == hub.name and
                                       not _h == hub for _h in saved_hubs):
                                    errors.append(
                                        ParseError(
                                            line_rule,
                                            """duplicate hub-names
                                            are not allowed"""))
                                    continue
                                elif any(_h.x == hub.x and
                                         _h.y == hub.y and not
                                         _h == hub for _h in saved_hubs):
                                    errors.append(ParseError(
                                        line_rule,
                                        """duplicate hub-coordinates
                                        are not allowed"""))
                                    continue
                                if (len(saved_hubs) > 0 and
                                   saved_hubs[0].position != "start"):
                                    errors.append(
                                        ParseError(
                                            line_rule,
                                            "normal hub before start_hub"
                                        )
                                    )
                                    continue
                                elif (any(
                                     hub.position == end
                                     for hub in saved_hubs) and
                                     saved_hubs[-1].position != "end"):
                                    errors.append(
                                        ParseError(
                                            line_rule,
                                            "normal hub after end_hub"
                                        )
                                    )
                                    continue
                                saved_hubs.append(hub)
                            elif key in "connection":
                                _values: list[str] = row.split(' ')
                                if not len(_values) < 3 or len(_values) == 0:
                                    errors.append(
                                        ParseError(
                                            line_rule,
                                            """invalid connection rule.
                                            A valid example:
                                            connection: hub1-hub2
                                            [max_link_capacity=4]"""
                                        )
                                    )
                                    continue
                                args["hub_1"] = None
                                args["hub_2"] = None
                                args["max_link_capacity"] = None
                                hubs: list[str] = _values[0].split('-')
                                if not len(hubs) == 2:
                                    errors.append(
                                        ParseError(
                                            line_rule,
                                            """A valid connection only
                                               consists out of two hubs"""))
                                    continue
                                if ((not any(_h.name == hubs[0]
                                    for _h in saved_hubs)) or
                                    (not any(_h.name == hubs[1]
                                     for _h in saved_hubs))):
                                    if not (any(_h.name == hubs[0]
                                            for _h in saved_hubs)):
                                        errors.append(
                                            ParseError(
                                                line_rule,
                                                f"non-existent hub '{hubs[0]}'"
                                            )
                                        )
                                    if not (any(_h.name == hubs[1]
                                            for _h in saved_hubs)):
                                        errors.append(
                                            ParseError(
                                                line_rule,
                                                f"non-existent hub '{hubs[1]}'"
                                            )
                                        )
                                    continue
                                args["hub_1"] = hubs[0]
                                args["hub_2"] = hubs[1]
                                if '[' in row and ']' in row:
                                    _values.pop(0)
                                    _values[0] = _values[0].strip().strip("[]")
                                    metas: list[str] = _values[0].split('=')
                                    if not len(metas) == 2:
                                        errors.append(
                                            ParseError(
                                                line_rule,
                                                "incorrect alignment found"
                                            )
                                        )
                                        continue
                                    if metas[0] in args:
                                        args[metas[0]] = metas[1]
                                _connection: Connection = Connection(
                                    hub1=args["hub_1"],
                                    hub2=args["hub_2"],
                                    max_link_capacity=args["max_link_capacity"]
                                )
                                if any([connection != _connection and
                                        connection.hub1 == _connection.hub1 and
                                        connection.hub2 == _connection.hub2
                                        for connection in saved_connections]):
                                    errors.append(
                                        ParseError(
                                            line_rule,
                                            f"""duplicate connection
                                                found [{hubs[0]}-{hubs[1]}]"""
                                        )
                                    )
                                    continue
                                saved_connections.append(_connection)
                            elif key in "nb_drones":
                                if not row.isdigit():
                                    errors.append(
                                        ParseError(
                                            line_rule,
                                            "nb_drones is not numeric"
                                        )
                                    )
                                    nb_drones = 0
                                    continue
                                elif row.isdigit() and int(row) < 1:
                                    errors.append(
                                        ParseError(
                                            line_rule,
                                            """nb_drones needs to
                                            be higher than zero"""
                                        )
                                    )
                                    continue
                                nb_drones = int(row)
            if nb_drones is None:
                errors.append(ParseError(None, "nb_drones is required"))
            if start < 1:
                errors.append(ParseError(None, "a start_hub is required"))
            if end < 1:
                errors.append(ParseError(None, "an end_hub is required"))
            if not len(errors) == 0:
                raise ParsingError(f"could not parse map {path}", errors)
            return Map(
                nb_drones=nb_drones,
                hubs=saved_hubs,
                connections=saved_connections
            )
        except ParsingError as err:
            print(err)
        except ValueError as err:
            print("lil' bug detected: ", err)
        except KeyError as err:
            print("pydantic doesn't like it: ", err)
        return None

    @staticmethod
    def prepare_output(_map: Map,
                       diff: str, name: str) -> dict[str, Any]:
        output: dict[str, Any] = {}
        output["name"] = name
        output["difficulty"] = diff
        output["nb_drones"] = _map.nb_drones
        output["hubs"] = [
            {
                "name": hub.name,
                "x": hub.x,
                "y": hub.y,
                "color": hub.color,
                "zone": hub.zone,
                "max_drones": hub.max_drones,
                "position": hub.position
            }
            for hub in _map.hubs
        ]
        output["connections"] = [
            {
                "hub_1": connection.hub1,
                "hub_2": connection.hub2,
                "max_link_capacity": connection.max_link_capacity
            }
            for connection in _map.connections
        ]
        return output

    @staticmethod
    def write_output(self, data: dict[str, Any]) -> bool:
        try:
            contents: str = dumps(data, indent=4)
            with open("result/map.json", "w", encoding="utf-8") as file:
                file.write(contents)
            return not len(self.__target) == 0
        except (FileNotFoundError,
                PermissionError, OSError, TypeError, ValueError) as err:
            print(err)
            return False

    @staticmethod
    def write_output_fail() -> None:
        try:
            tab: str = "    "
            contents: str = f"{{\n{tab}\"could not parse the chosen map\"\n}}"
            with open("result/fail.json", 'w', encoding="utf-8") as file:
                file.write(contents)
        except (FileNotFoundError,
                PermissionError, OSError, TypeError, ValueError) as err:
            print(err)
