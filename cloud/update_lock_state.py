from pydantic import BaseModel
from error import error_handle
from models import LockState
from iot_shadow import get_shadow_state, update_shadow_state


class LockStateUpdateRequestBody(BaseModel):
    lock: str


@error_handle(success_handle=lambda x: {"message": "Lock state updated successfully"})
def update_lock_state(event, context):
    body = LockStateUpdateRequestBody.model_validate_json(event.get("body", "{}"))

    newLockState = LockState[body.lock]

    # Distributed critical section -- may need to think this out more later (would be ridicuosly unlucky for timing to break things)
    shadow_state = get_shadow_state()
    shadow_state.lock = newLockState
    update_shadow_state(shadow_state)
