from abc import ABCMeta, abstractmethod

from speculos.client import SpeculosClient, ApduException
from ledgercomm import Transport


class ClientInterface(metaclass=ABCMeta):
    @abstractmethod
    def apdu_exchange_with_buttons(self, cla, ins, p1, p2, cdata, n_screens):
        ...

    @abstractmethod
    def apdu_exchange_raw_with_buttons(self, raw, n_screens):
        ...

    @abstractmethod
    def apdu_exchange_raw(self, raw):
        ...

    @abstractmethod
    def apdu_exchange(self, cla, ins, p1, p2, cdata):
        ...

    @abstractmethod
    def start(self):
        ...

    @abstractmethod
    def close(self):
        ...


class Speculos(ClientInterface):
    def __init__(self, file_path: str, args, automatic) -> None:
        self.client = SpeculosClient(app=file_path, args=args)
        self.automatic = automatic

    def apdu_exchange_with_buttons(self, cla, ins, p1, p2, cdata, n_screens):
        with self.client.apdu_exchange_nowait(
            cla=cla, ins=ins, p1=p1, p2=p2, data=cdata
        ) as exchange:
            if self.automatic:
                for _ in range(n_screens):
                    self.client.press_and_release("right")
                # Approve
                self.client.press_and_release("both")
            return exchange.receive()

    def apdu_exchange_raw_with_buttons(self, raw, n_screens):
        return self.apdu_exchange_with_buttons(
            cla=raw[0] if len(raw) > 0 else b"",
            ins=raw[1] if len(raw) > 1 else b"",
            p1=raw[2] if len(raw) > 2 else b"",
            p2=raw[3] if len(raw) > 3 else b"",
            cdata=raw[5:] if len(raw) > 5 else b"",
            n_screens=n_screens,
        )

    def apdu_exchange_raw(self, raw):
        return self.client._apdu_exchange(
            bytes.fromhex(raw) if isinstance(raw, str) else raw
        )

    def apdu_exchange(self, cla, ins, p1, p2, cdata):
        return self.client.apdu_exchange(cla=cla, ins=ins, p1=p1, p2=p2, data=cdata)

    def start(self):
        self.client.start()

    def close(self):
        self.client.stop()


class Ledgercomm(ClientInterface):
    def __init__(self) -> None:
        self.transport = Transport(interface="hid", debug=True)

    def apdu_exchange_with_buttons(self, cla, ins, p1, p2, cdata, n_screens):
        return self.apdu_exchange(cla, ins, p1, p2, cdata)

    def apdu_exchange_raw_with_buttons(self, raw, n_screens):
        return self.apdu_exchange_raw(raw)

    def apdu_exchange_raw(self, raw):
        sw, response = self.transport.exchange_raw(raw)

        if sw != 0x9000:
            raise ApduException(sw=sw, data=response)

        return response

    def apdu_exchange(self, cla, ins, p1, p2, cdata):
        sw, response = self.transport.exchange(
            cla=cla, ins=ins, p1=p1, p2=p2, cdata=cdata
        )

        if sw != 0x9000:
            raise ApduException(sw=sw, data=response)

        return response

    def start(self):
        pass

    def close(self):
        pass
