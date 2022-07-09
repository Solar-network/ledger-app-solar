from btclib.ecc import ssa
import pytest
from client.exception import *
from speculos.client import ApduException
from client.cmd_builder import InsType


message1="We are the music makers,\n the dreamers of dreams...\n"

message2="[T]he phrase \"debugging a computer\" was coined in 1947 when Harvard researchers found a moth crushed between the jaws of a relay switch in a Mark II Aiken Relay Calculator. The dead moth was taped to the machine's logbook, which now resides at the National Museum of American History of the Smithsonian Institution.\r\nWhenever the machine broke down, which was often, operators would quip that they were \"debugging\" it.\r\n"

message3="The size and age of the Cosmos are beyond ordinary human understanding. Lost somewhere between immensity and eternity is our tiny planetary home. In a cosmic perspective, most human concerns seem insignificant, even petty. And yet our species is young and curious and brave and shows much promise. In the last few millennia we have made the most astonishing and unexpected discoveries about the Cosmos and our place within it, explorations that are exhilarating to consider. They remind us that humans have evolved to wonder, that understanding is a joy, that knowledge is prerequisite to survival. I believe our future depends on how well we know this Cosmos in which we float like a mote of dust in the morning sky."

message4="A man is flying in a hot air balloon and realizes he is lost. He reduces height and spots a man down below. He lowers the balloon further and shouts: \"Excuse me, can you tell me where I am?\"\nThe man below says: \"Yes you're in a hot air balloon, hovering 30 feet above this field.\"\n\"You must be a software developer,\" says the balloonist.\n\"I am,\" replies the man. \"How did you know?\"\n\"Well,\" says the balloonist, \"everything you have told me is technically correct, but it's of no use to anyone.\"\nThe man below says, \"You must work in business as a manager.\"\n\"I do,\" replies the balloonist, \"but how did you know?\"\n\"Well,\" says the man, \"you don't know where you are or where you are going, but you expect me to be able to help. You're in the same position you were before we met but now it's my fault.\""

message5="\"Okay. About twenty years ago A.T.&T. made a multi-billion-dollar decision to operate its entire long-distance switching system on twelve electronically generated combinations of twelve master tones. Those are the tones you sometimes hear in the background after you've dialed a long-distance number. They decided to use some very simple tones -- the tone for each number is just two fixed single-frequency tones played simultaneously to create a certain beat frequency. Like 1300 cycles per second and 900 cycles per second played together give you the tone for digit 5. Now, what some of these phone phreaks have done is get themselves access to an electric organ. Any cheap family home-entertainment organ. Since the frequencies are public knowledge now -- one blind phone phreak has even had them recorded in one of the talking books for the blind -- they just have to find the musical notes on the organ which correspond to the phone tones. Then they tape them. For instance, to get Ma Bell's tone for the number 1, you press down organ keys F~5 and A~5 (900 and 700 cycles per second) at the same time. To produce the tone for 2 it's F~5 and C~6 (1100 and 700 c.p.s). The phone phreaks circulate the whole list of notes so there's no trial and error anymore.\""

message6="Man is the Reasoning Animal. Such is the claim. I think it is open to dispute. Indeed, my experiments have proven to me that he is the Unreasoning Animal... In truth, man is incurably foolish. Simple things which other animals easily learn, he is incapable of learning. Among my experiments was this. In an hour I taught a cat and a dog to be friends. I put them in a cage. In another hour I taught them to be friends with a rabbit. In the course of two days I was able to add a fox, a goose, a squirrel and some doves. Finally a monkey. They lived together in peace; even affectionately.\n Next, in another cage I confined an Irish Catholic from Tipperary, and as soon as he seemed tame I added a Scotch Presbyterian from Aberdeen. Next a Turk from Constantinople; a Greek Christian from Crete; an Armenian; a Methodist from the wilds of Arkansas; a Buddhist from China; a Brahman from Benares. Finally, a Salvation Army Colonel from Wapping. Then I stayed away for two whole days. When I came back to note results, the cage of Higher Animals was all right, but in the other there was but a chaos of gory odds and ends of turbans and fezzes and plaids and bones and flesh--not a specimen left alive. These Reasoning Animals had disagreed on a theological detail and carried the matter to a Higher Court."

message7="A long, long time ago the World was in an age of Chaos. In the midst of the chaos, in a little kingdom in the land of Hyrule, a legend was being handed down from generation to generation, the legend of the 'Triforce': golden triangles possessing mystical powers. One day, an evil army led by Ganon, the powerful Prince of Darkness, attacked and stole the Triforce of Power. Fearing his wicked rule, princess Zelda split the Triforce of Wisdom into eight fragments and hid them throughout the realm. She commanded her most trustworthy nursemaid, Impa, to escape and search for someone courageous enough to destroy the evil Ganon. Upon hearing this, Ganon grew angry, imprisoned the princess, and sent out a party in search of Impa. Braving forests and mountains, Impa fled for her life from her pursuers. As she reached the very limit of her energy she found herself surrounded by Ganon's evil henchmen. Cornered! What could she do? But wait! All was not lost. A young lad appeared, driving off Ganon's henchmen and saving Impa from a fate worse than death. His name was Link. Impa told Link the story of Zelda and the evil Ganon. Burning with a sense of justice, Link resolved to save Zelda. But Ganon was a powerful opponent. He held the Triforce of Power. Link had to bring the scattered eight fragments of the Triforce of Wisdom together to rebuild the mystical triangle or there would be no chance he could fight his way into Death Mountain where Ganon lived. Can Link really destroy Ganon and save princess Zelda? Only your skill can answer that question. Good luck. Use the Triforce wisely."


@pytest.fixture(autouse=True)
def context(cmd):
    bip32_path: str = "44'/3333'/0'/0/0"

    pub_key, chain_code = cmd.get_public_key(
        bip32_path=bip32_path, display=0, chaincode=0
    )

    context = {"pkey": pub_key, "chain_code": chain_code, "bip32": bip32_path}
    yield context


def test_sign_message(cmd, context):

    schnorr_sig = cmd.sign_message(
        bip32_path=context["bip32"], message=message1, n_screens=5
    )
    assert (
        ssa.verify(message1.encode("ascii"), context["pkey"][1:], schnorr_sig) is True
    )

    schnorr_sig = cmd.sign_message(
        bip32_path=context["bip32"], message=message2, n_screens=26
    )
    assert (
        ssa.verify(message2.encode("ascii"), context["pkey"][1:], schnorr_sig) is True
    )

    schnorr_sig = cmd.sign_message(
        bip32_path=context["bip32"], message=message3, n_screens=43
    )
    assert (
        ssa.verify(message3.encode("ascii"), context["pkey"][1:], schnorr_sig) is True
    )

    schnorr_sig = cmd.sign_message(
        bip32_path=context["bip32"], message=message4, n_screens=45
    )
    assert (
        ssa.verify(message4.encode("ascii"), context["pkey"][1:], schnorr_sig) is True
    )

    schnorr_sig = cmd.sign_message(
        bip32_path=context["bip32"], message=message5, n_screens=69
    )
    assert (
        ssa.verify(message5.encode("ascii"), context["pkey"][1:], schnorr_sig) is True
    )

    schnorr_sig = cmd.sign_message(
        bip32_path=context["bip32"], message=message6, n_screens=70
    )
    assert (
        ssa.verify(message6.encode("ascii"), context["pkey"][1:], schnorr_sig) is True
    )

    schnorr_sig = cmd.sign_message(
        bip32_path=context["bip32"], message=message7, n_screens=89
    )
    assert (
        ssa.verify(message7.encode("ascii"), context["pkey"][1:], schnorr_sig) is True
    )


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_too_long_message(cmd, context):
    try:
        message = "a" * 1598

        cmd.sign_message(bip32_path=context["bip32"], message=message)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_MESSAGE)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_not_ascii(cmd, context):
    try:
        message = "\0this should not work"

        cmd.sign_message(bip32_path=context["bip32"], message=message)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_MESSAGE)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_not_ascii2(cmd, context):
    try:
        message = "this should not work\r"

        cmd.sign_message(bip32_path=context["bip32"], message=message)
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_MESSAGE)


@pytest.mark.xfail(raises=TxParsingFailError, strict=True)
def test_length_validation(cmd):
    try:
        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC1,
            p1=0x00,
            p2=0x80,
            cdata=bytes.fromhex("058000002c80000d05800000000000000000000000"),
        )

        cmd.transport.apdu_exchange(
            cla=0xE0,
            ins=0xC1,
            p1=0x01,
            p2=0x00,
            cdata=bytes.fromhex("fe00" + "63" * 253),
        )
    except ApduException as error:
        raise DeviceException(error_code=error.sw, ins=InsType.INS_SIGN_MESSAGE)
