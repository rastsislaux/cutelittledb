#! /bin/python3.10
import json
import sys
from CuteLittle import Client, Request
from dataclasses import dataclass


@dataclass
class TestObject:
    name: str
    age: int
    jobs: list


def main(args: list):
    db = Client()
    db.connect("localhost", int(args[1]))

    data = 0
    if args[2] == "put":
        data = db.request(Request(
            Request.Command.PUT,
            "testCollection",
            TestObject("Oleg", 25, ["Carpenter", "QA"])
        ))

    if args[2] == "get":
        data = db.request(Request(
            Request.Command.GET,
            "testCollection",
            0
        ))

    db.disconnect()
    print(data)


if __name__ == "__main__":
    main(sys.argv)
