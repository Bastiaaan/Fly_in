
from typing import NamedTuple


class ParseError(NamedTuple):
    line: int | None
    message: str


class ParsingError(Exception):
    """A customized error object which is
    specialized in making errors for parsing related problems"""
    def __init__(self, msg: str, errors: list[ParseError]) -> None:
        errTemplate: str = msg
        errTemplate += "\n" + "\n".join(
            f"- {error}" + ("" if line is None else f" at line {line}")
            for (line, error) in errors
        )
        super().__init__(errTemplate)
