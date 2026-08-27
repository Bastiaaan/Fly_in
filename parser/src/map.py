
from pydantic import BaseModel, model_validator, Field
from .connection import Connection
from .hub import Hub


class Map(BaseModel):
    nb_drones: int = Field(ge=1)
    hubs: list[Hub]
    connections: list[Connection]

    @model_validator(mode="after")
    def validate_rules(self) -> "Map":
        if self.nb_drones is None:
            raise KeyError("nb_drones cannot be empty")
        elif self.nb_drones <= 0:
            raise KeyError("nb_drones cannot be negative or zero")
        return self
