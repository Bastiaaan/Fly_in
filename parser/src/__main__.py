
from typing import Any
from .map import Map
from .map_manager import MapManager
from .parser import Parser


def main() -> None:
    print("\nWelcome to Fly_in!")

    manager: MapManager = MapManager()
    diff: str = manager.input_difficulty()
    files: list[str] = manager.load_maps(diff)
    formatted: dict[int, str] = manager.show_maps_ui(files, diff)
    mapNum: int = int(input("choose a map: "))
    while mapNum not in formatted:
        mapNum = int(input("invalid choice. try again: "))
    target_path: str = f"../maps/{diff}/{files[mapNum-1]}"
    _map: Map | None = Parser.parse_map(target_path)
    Parser.check_directory()
    if _map is None:
        Parser.write_output_fail()
    else:
        _json: dict[str, Any] = Parser.prepare_output(
            _map, diff, formatted[mapNum])
        Parser.write_output(Parser, _json)


if __name__ == "__main__":
    main()
