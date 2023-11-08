"""
Module for managing AWS IoT 'smart-handle' thing shadow state via boto3.

Provides functions to get and update the desired state of the IoT thing shadow in AWS IoT's cloud infrastructure,
using the ShadowState model for data validation and serialization.

Functions:
    get_shadow_state: Retrieves and validates the current desired shadow state.
    update_shadow_state: Updates the desired shadow state with new values.
"""
import boto3
import json
from models import ShadowState

IOT_CLIENT = boto3.client("iot-data", region_name="us-east-2")
IOT_THING_NAME = "smart-handle"

def get_shadow_state() -> ShadowState:
    """
    Retrieve the desired shadow state of the specified IoT thing.

    This function calls the AWS IoT Data Plane to get the shadow state of the IoT thing
    named 'smart-handle'. It parses the returned JSON payload to extract the 'desired'
    state and validates it using the ShadowState model's validation method.

    Returns:
        ShadowState: An instance of the ShadowState model containing the desired state of the IoT thing.

    Raises:
        json.JSONDecodeError: If there's an error in decoding the JSON payload.
        KeyError: If the expected keys are not found in the JSON payload.
        ValidationError: If the shadow state received fails pydantic validation for ShadowState
    """
    response = IOT_CLIENT.get_thing_shadow(thingName=IOT_THING_NAME)

    # The response payload is a streamingBody object. Read it into a variable
    streaming_body = response["payload"]
    get_response = json.loads(streaming_body.read())
    shadow_state_dict: dict = get_response["state"]["desired"]

    return ShadowState.model_validate(shadow_state_dict)

def update_shadow_state(desired: ShadowState):
    """
    Update the desired shadow state of the specified IoT thing.

    Constructs a JSON payload with the desired state provided by the ShadowState model's
    dump method. The payload is then used to update the shadow state of the IoT thing
    named 'smart-handle' through the AWS IoT Data Plane service.

    Args:
        desired (ShadowState): An instance of ShadowState model with the desired state configuration.

    Returns:
        None: The function returns nothing. It updates the state on AWS IoT and any result is handled by AWS services.

    Raises:
        ClientError: If there's an error in the client's request to AWS IoT Data Plane.
    """
    update_payload = f"{{\"state\":{{\"desired\":{desired.model_dump_json()}}}}}"
    # Update the shadow
    IOT_CLIENT.update_thing_shadow(thingName=IOT_THING_NAME, payload=update_payload)
