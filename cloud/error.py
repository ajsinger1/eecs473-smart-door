import json

def response(code, body_json):
    return {
        "statusCode": code,
        "body": json.dumps(body_json),
        "headers": {"content-type": "application/json"},
    }


def error_handle(success_handle = None, error_message: str = "Internal Server Error (lambda)"):
    if success_handle is None:
        def success_handle(_):
            return {"message": "SUCCESS"}
    def decorator(lambda_function_handler):
        def wrapper(body, context):
            try:
                return response(200, success_handle(lambda_function_handler(body, context)))
            except Exception as e:
                print(e)
                return response(500, {"error": error_message})
        return wrapper
    return decorator
