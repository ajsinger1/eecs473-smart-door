"""
Defines data models and enums for representing and validating the state of IoT device shadows.

This module uses Pydantic for data validation, ensuring that the IoT device states
conform to the specified schema before being processed or transmitted to other services.

Available Classes:
- Mode: Enum for IoT device operation modes.
- LockState: Enum for lock states of an IoT device.
- UserState: Enum for defining possible user interaction states with the IoT device.
- UserTableItem: Data model for user information storage.
- ShadowUser: Data model for representing a user's shadow on an IoT device.
- ShadowState: Data model for the composite state of an IoT device including user shadows.

These models are used to structure data consistently for the IoT device management system.
"""

from pydantic import BaseModel
from typing import Optional, List
from enum import IntEnum, Enum

class Mode(IntEnum):
    """
    Smart Handle modes
    """
    SECURE = 0
    PROXIMITY = 1


class LockState(IntEnum):
    """
    Smart Handle lock states
    """
    UNLOCKED = 0
    LOCKED = 1


class UserState(IntEnum):
    """
    Smart Handle user states
    """
    UNLOCK = 0
    DO_NOT_UNLOCK = 1
    LOCKOUT = 2


class UserTableItem(BaseModel):
    """
    A model representing an item in the user table, detailing user identification and status.

    Attributes:
        uuid (str): A unique identifier for the user.
        name (Optional[str]): The full name of the user; optional.
        username (Optional[str]): The username for the user; optional.
        password (Optional[str]): The password for the user; optional.
        is_sudo (Optional[bool]): Flag indicating if the user has sudo privileges; optional.
    """
    uuid: str
    name: Optional[str] = None
    username: Optional[str] = None
    password: Optional[str] = None
    is_sudo: Optional[bool] = None


class ShadowUser(BaseModel):
    """
    A model for representing a user's state in the device shadow.

    Attributes:
        uuid (str): A unique identifier for the user.
        state (UserState): The current state of the user with respect to the IoT device.
    """
    uuid: str
    state: UserState


class ShadowState(BaseModel):
    """
    A model defining the overall shadow state of an IoT device, containing modes and user states.

    Attributes:
        mode (Mode): The current mode of operation for the device.
        lock (LockState): The current lock state of the device.
        users (List[ShadowUser]): A list of `ShadowUser` representing the state of users in the shadow.
    """
    mode: Mode
    lock: LockState
    users: List[ShadowUser]

