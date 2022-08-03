import socket
import json
import jsons
from enum import Enum, auto


class Request:
    class Command(Enum):
        PUT = auto()
        GET = auto()

    cmd: Command    # Command to execute
    cll: str        # Collection name
                    # Object to send

    def __init__(self, command, collection, obj=None):
        self.cmd = command
        self.cll = collection
        self.obj = obj


class Client:
    _addr: str
    _port: int
    _sock: any

    def __init__(self):
        pass

    def connect(self, addr: str, port: int):
        self._addr = addr
        self._port = port
        self._sock = socket.socket()
        self._sock.connect((self._addr, self._port))

    def disconnect(self):
        self._sock.close()

    def raw_request(self, request: str):
        self._sock.send(bytes(request, 'ascii'))
        s = self._cstr(self._sock.recv(4096))
        return json.loads(s)

    def request(self, request: Request):
        self.raw_request(jsons.dumps(request, strip_privates=True))

    @staticmethod
    def _cstr(x: bytes) -> str:
        s = x.find(0)
        return str(x[:s], 'ascii')
