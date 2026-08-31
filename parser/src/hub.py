
from pydantic import BaseModel, Field, model_validator
from typing import Optional


class Hub(BaseModel):
    """The crucial landing zones for the drones within the simulator."""

    name: str = Field(min_length=1)
    x: int = Field(ge=0)
    y: int = Field(default=0)
    color: Optional[str] = Field(default=None)
    max_drones: Optional[int] = Field(default=None)
    zone: Optional[str] = Field(default=None)
    position: Optional[str] = Field(default=None)

    @model_validator(mode="after")
    def validate_rules(self) -> "Hub":
        if not self.name:
            raise ValueError("hub name is required")
        if self.x < 0:
            raise ValueError("hub x-coord cannot be negative")

        if self.color is None:
            self.color = "gray"
        if self.zone is None:
            self.zone = "normal"
        if self.max_drones is None:
            self.max_drones = 1
        if self.position is None:
            self.position = "midway"

        return self
