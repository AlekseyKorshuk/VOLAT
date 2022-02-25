import json


class BytesConverter:
    """Converts bytes to string and vice-versa"""

    def del_none(self, dictionary: dict) -> dict:
        """
        Delete keys with the value ``None`` in a dictionary, recursively.
        This alters the input so you may wish to ``copy`` the dict first.

        @param dictionary: Dictionary
        @return: Resulting dictionary
        """

        for key, value in list(dictionary.items()):
            if value is None:
                del dictionary[key]
            elif isinstance(value, dict):
                self.del_none(value)
        return dictionary

    def _int_to_bytes(self, value: int, bytes_size: int = 4) -> bytes:
        """
        Converts integer into bytes
        @param value: Integer to convert
        @param bytes_size: Bytes size
        @return: Bytes
        """
        return value.to_bytes(bytes_size, byteorder='little')

    def generate_message(self, action: int, message: dict = None) -> bytes:
        """
        Generates bytes message from dictionary
        @param action: Action type
        @param message: Dictionary
        @return: Bytes
        """
        message_length = 0
        if message is not None:
            message = self.del_none(message.copy())
            message = json.dumps(message, separators=(',', ':'))
            message_length = len(str(message))

        array = [
            self._int_to_bytes(action),
            self._int_to_bytes(message_length),
        ]
        if message_length - 2 > 0:
            array.append(
                bytes(message, encoding='utf-8')
            )
        return b"".join(
            array
        )

    def __call__(self, action: int, message: dict = None) -> bytes:
        """
        Call method of the class.
        Generates bytes message from dictionary
        @param action: Action type
        @param message: Dictionary
        @return: Bytes
        """
        return self.generate_message(action, message)

    def message_to_dict(self, result_code: int, message: bytes) -> dict:
        """
        Converts bytes message to dictionary
        @param result_code: Integer result code
        @param message: Bytes message
        @return: Dictionary
        """
        response = {
            'result_code': result_code,
        }
        try:
            response.update(
                json.loads(str(message)[2:-1])
            )
        except Exception as ex:
            # Handle any error
            print(ex)
            response['data'] = str(message)[2:-1]
        return response
