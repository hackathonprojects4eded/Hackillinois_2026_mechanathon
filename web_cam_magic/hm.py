import asyncio
from bleak import BleakClient

ADDRESS = "70DD0F8B-57EB-4F1F-6A0F-23EA21595677"


async def main():
    async with BleakClient(ADDRESS) as client:
        print(f"Connected: {client.is_connected}")

        for service in client.services:
            print(f"\nService: {service.uuid}")
            for char in service.characteristics:
                print(f"  Char: {char.uuid}")
                print(f"  Properties: {char.properties}")


asyncio.run(main())
