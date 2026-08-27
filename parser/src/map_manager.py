
from .hub import Hub
from pathlib import Path


class MapManager:
    """after Pydantic's passes for each components
       this class will parse all the results into a JSON-file to
       ready the map for the algorithm/rendering of the C++'s side"""

    def __init__(self) -> None:
        self.found_hubs: list[Hub] = list[Hub]()

        order: list[str] = ["easy", "medium", "hard", "challenger"]
        self._map_path: Path = Path("../maps")
        self.available_diffs: list[str] = sorted(
            [diff.name for diff in self._map_path.iterdir() if diff.is_dir()],
            key=lambda d: order.index(d) if d in order else -1
        )

    def input_difficulty(self) -> str:
        print("\n\n========== choose a difficulty: ==========\n\n")
        for _, diff in enumerate(self.available_diffs, start=1):
            print(f"({_}): {diff}")
        #  extra space
        print()
        intput: int = int(input("my choice: "))
        while intput not in range(1, len(self.available_diffs) + 1):
            print(f"{intput} is not a valid choice. try again")
            intput = int(input())
        selected: str = self.available_diffs[intput - 1]
        return selected

    def load_maps(self, diff: str) -> list[str]:
        path: Path = self._map_path / diff
        files: list[str] = sorted(
            found.name for found in path.iterdir()
        )
        return files

    def show_maps_ui(self, files: list[str], diff: str) -> dict[int, str]:
        formatted: dict[int, str] = {
            index: file.split("_", 1)[1].rsplit(".", 1)[0]
                                        .replace("_", " ")
                                        .capitalize()
            for index, file in enumerate(files, start=1)
            if file.endswith(".txt")
        }
        print(f"\n======== Available maps ({diff}): ========\n")
        for index, name in formatted.items():
            print(f"{index}: {name}")
        return formatted
