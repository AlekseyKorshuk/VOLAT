import json


class BytesConverter:

    def __init__(self):
        pass

    def del_none(self, d):
        """
        Delete keys with the value ``None`` in a dictionary, recursively.

        This alters the input so you may wish to ``copy`` the dict first.
        """
        for key, value in list(d.items()):
            if value is None:
                del d[key]
            elif isinstance(value, dict):
                self.del_none(value)
        return d

    def _int_to_bytes(self, value: int, bytes_size: int = 4) -> bytes:
        return value.to_bytes(bytes_size, byteorder='little')

    def generate_message(self, action: int, message: dict = '') -> bytes:
        if message is not '':
            message = self.del_none(message.copy())
            message = json.dumps(message, separators=(',', ':'))
            message_length = len(str(message))
        else:
            message_length = 0

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

    def __call__(self, action: int, message: dict = '') -> bytes:
        return self.generate_message(action, message)

    def message_to_dict(self, result_code: int, message: bytes) -> dict:
        response = {
            'result_code': result_code,
        }
        try:
            response.update(
                json.loads(str(message)[2:-1])
            )
        except Exception as ex:
            response['data'] = str(message)[2:-1]
        return response
