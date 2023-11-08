from pydantic import BaseModel
from error import error_handle
from models import Mode
from iot_shadow import get_shadow_state, update_shadow_state

class GetLockResponseBody(BaseModel):
    lock: str

@error_handle(success_handle=lambda x: x)
def get_lock_state(event, context):
    shadow_state = get_shadow_state()
    return GetLockResponseBody(lock=shadow_state.lock.name).model_dump()
