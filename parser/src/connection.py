
from pydantic import BaseModel, Field, model_validator
from typing import Optional


class Connection(BaseModel):
    """A crucial object which is required for the drones to fly"""

    hub1: str = Field(min_length=1)
    hub2: str = Field(min_length=1)
    max_link_capacity: Optional[int] = Field(default=None)

    @model_validator(mode="after")
    def validate_rules(self) -> "Connection":
        if self.hub1 is None:
            raise ValueError("hub- is required")
        elif self.hub2 is None:
            raise ValueError("-hub is required")

        if self.max_link_capacity is None:
            self.max_link_capacity = 1
        return self
