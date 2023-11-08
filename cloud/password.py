import bcrypt

def encrypt_password(password_plain):
    password_plain_bytes = password_plain.encode("utf-8")
    salt = bcrypt.gensalt()  # Generate a salt
    return bcrypt.hashpw(password_plain_bytes, salt).decode("utf-8")