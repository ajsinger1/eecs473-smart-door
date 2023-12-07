"""
Declares encryption functionality.
"""
import bcrypt

def encrypt_password(password_plain):
    """Salts and encrypts plaintext, returning it as a utf-8 bytestring"""
    password_plain_bytes = password_plain.encode("utf-8")
    salt = bcrypt.gensalt()  # Generate a salt
    return bcrypt.hashpw(password_plain_bytes, salt).decode("utf-8")