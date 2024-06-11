from enum import Enum, auto

FREQ_DIV = 1e6


class ArithmeticEnum(Enum):
    def __add__(self, other):
        if isinstance(other, self.__class__):
            return self.__class__((self.value + other.value) % len(self.__class__))
        return NotImplemented

    def __sub__(self, other):
        if isinstance(other, self.__class__):
            return self.__class__((self.value - other.value) % len(self.__class__))
        return NotImplemented


class Notes(ArithmeticEnum):
    O_ = 0
    
    C2 = auto()
    CD2 = auto()
    D2 = auto()
    DE2 = auto()
    E2 = auto()
    F2 = auto()
    FG2 = auto()
    G2 = auto()
    GA2 = auto()
    A2 = auto()
    AB2 = auto()
    B2 = auto()
    C3 = auto()

    CD3 = auto()
    D3 = auto()
    DE3 = auto()
    E3 = auto()
    F3 = auto()
    FG3 = auto()
    G3 = auto()
    GA3 = auto()
    A3 = auto()
    AB3 = auto()
    B3 = auto()

    C4 = auto()
    CD4 = auto()
    D4 = auto()
    DE4 = auto()
    E4 = auto()
    F4 = auto()
    FG4 = auto()
    G4 = auto()
    GA4 = auto()
    A4 = auto()
    AB4 = auto()
    B4 = auto()

    C5 = auto()
    CD5 = auto()
    D5 = auto()
    DE5 = auto()
    E5 = auto()
    F5 = auto()
    FG5 = auto()
    G5 = auto()
    GA5 = auto()
    A5 = auto()
    AB5 = auto()
    B5 = auto()

ONE = Notes.C2

# print(Notes.O_.name)
# print(Notes.O_.value)

# print(Notes.C2.name)
# print(Notes.C2.value)

class Score:
    def __init__(self, content, go_flat, name) -> None:
        self.content = content
        self.go_flat = go_flat
        self.name = name

# ==================================

names = ["anthem", "BWV846", "Ievan_polkka"]




anthem_content = [
    [[Notes.E5, 3], [Notes.O_, 0.25], [Notes.B4, 0.25]],
    [[Notes.E5, 1], [Notes.B4, 0.75], [Notes.C5, 0.25], [Notes.D5, 1.0], [Notes.G4, 0.50], [Notes.G4, 0.50]],
    [[Notes.C5, 1], [Notes.B4, 0.75], [Notes.A4, 0.25], [Notes.B4, 1.0], [Notes.E4, 0.50], [Notes.E4, 0.50]],
    [[Notes.F4, 1], [Notes.F4, 0.75], [Notes.G4, 0.25], [Notes.A4, 1.0], [Notes.A4, 0.75], [Notes.B4, 0.25]],
    [[Notes.C5, 1], [Notes.D5, 0.50], [Notes.E5, 0.50], [Notes.F5, 1.5], [Notes.B4, 0.50]],
    [[Notes.G5, 1], [Notes.F5, 0.75], [Notes.E5, 0.25], [Notes.F5, 1.0], [Notes.D5, 0.50], [Notes.B4, 0.50]],
    [[Notes.E5, 1], [Notes.D5, 0.75], [Notes.C5, 0.25], [Notes.D5, 1.0], [Notes.G4, 0.50], [Notes.G4, 0.50]],
    [[Notes.C5, 1], [Notes.B4, 0.75], [Notes.A4, 0.25], [Notes.B4, 1.0], [Notes.E4, 0.75], [Notes.E4, 0.25]],
    [[Notes.E5, 1], [Notes.D5, 0.75], [Notes.C5, 0.25], [Notes.B4, 1.5], [Notes.O_, 0.50]],
]
anthem = Score(anthem_content, ["A", "B", "E"], names[0])

BWV846_content = [
    [[Notes.C4, 0.25], [Notes.E4, 0.25], [Notes.G4, 0.25], [Notes.C5, 0.25], [Notes.E5, 0.25], [Notes.G4, 0.25], [Notes.C5, 0.25], [Notes.E5, 0.25]],
    [[Notes.C4, 0.25], [Notes.E4, 0.25], [Notes.G4, 0.25], [Notes.C5, 0.25], [Notes.E5, 0.25], [Notes.G4, 0.25], [Notes.C5, 0.25], [Notes.E5, 0.25]],

    [[Notes.C4, 0.25], [Notes.E4 - ONE, 0.25], [Notes.G4 + ONE, 0.25], [Notes.C5 + ONE, 0.25], [Notes.E5 + ONE, 0.25], [Notes.G4 + ONE, 0.25], [Notes.C5 + ONE, 0.25], [Notes.E5 + ONE, 0.25]],
    [[Notes.C4, 0.25], [Notes.E4 - ONE, 0.25], [Notes.G4 + ONE, 0.25], [Notes.C5 + ONE, 0.25], [Notes.E5 + ONE, 0.25], [Notes.G4 + ONE, 0.25], [Notes.C5 + ONE, 0.25], [Notes.E5 + ONE, 0.25]],

    [[Notes.C4 - ONE, 0.25], [Notes.E4 - ONE, 0.25], [Notes.G4, 0.25], [Notes.C5 + ONE, 0.25], [Notes.E5 + ONE, 0.25], [Notes.G4, 0.25], [Notes.C5 + ONE, 0.25], [Notes.E5 + ONE, 0.25]],
    [[Notes.C4 - ONE, 0.25], [Notes.E4 - ONE, 0.25], [Notes.G4, 0.25], [Notes.C5 + ONE, 0.25], [Notes.E5 + ONE, 0.25], [Notes.G4, 0.25], [Notes.C5 + ONE, 0.25], [Notes.E5 + ONE, 0.25]],
]

BWV846 = Score(BWV846_content, [], names[1])

Ievan_polkka_content = [
    [[Notes.A5, 0.5], [Notes.A5, 0.5], [Notes.G5, 0.5], [Notes.F5, 0.25], [Notes.F5, 0.25], 
        [Notes.E5, 0.5], [Notes.C5, 0.25], [Notes.C5, 0.75], [Notes.E5, 0.5]],
    [[Notes.G5, 0.5], [Notes.G5, 0.25], [Notes.G5, 0.25], [Notes.F5, 0.5], [Notes.E5, 0.5], 
        [Notes.E5 + ONE, 0.5], [Notes.C5 + ONE, 0.5], [Notes.C5 + ONE, 0.5], [Notes.E5 + ONE, 0.5]],
    [[Notes.A5, 0.25], [Notes.A5, 0.25], [Notes.A5, 0.25], [Notes.A5, 0.25], [Notes.G5, 0.5], [Notes.F5, 0.5],
        [Notes.E5, 0.5], [Notes.C5, 0.25], [Notes.C5, 0.5], [Notes.C5, 0.25], [Notes.E5, 0.5]],
    [[Notes.G5, 0.25], [Notes.G5, 0.25], [Notes.G5, 0.5], [Notes.F5, 0.5], [Notes.E5, 0.25], [Notes.E5, 0.25],
        [Notes.F5, 0.5], [Notes.D5, 0.25], [Notes.D5, 1], [Notes.O_, 0.25]],
    
    [[Notes.A4, 0.5], [Notes.D5, 0.5], [Notes.D5, 0.75], [Notes.E5, 0.25],
        [Notes.F5, 0.25], [Notes.F5, 0.25], [Notes.D5, 0.25], [Notes.D5, 0.25], [Notes.D5, 0.5], [Notes.F5, 0.25], [Notes.F5, 0.25]],
    [[Notes.E5, 0.5], [Notes.C5, 0.5], [Notes.C5, 0.5], [Notes.E5, 0.5],
        [Notes.F5, 0.5], [Notes.D5, 0.5], [Notes.D5, 0.5], [Notes.D5, 0.25], [Notes.D5, 0.25]],
    [[Notes.A4, 0.5], [Notes.D5, 0.5], [Notes.D5, 0.75], [Notes.E5, 0.25],
        [Notes.F5, 0.25], [Notes.O_, 0.25], [Notes.D5, 0.5], [Notes.D5, 0.25], [Notes.D5, 0.25], [Notes.D5, 0.25], [Notes.F5, 0.25]],

    [[Notes.A5, 0.25], [Notes.A5, 0.25], [Notes.A5, 0.25], [Notes.G5, 0.25],
        [Notes.F5, 0.25], [Notes.O_, 0.25], [Notes.E5, 0.5],
        [Notes.F5, 0.5], [Notes.D5, 0.25], [Notes.O_, 0.25],
        [Notes.D5, 0.5], [Notes.D5, 0.25], [Notes.D5, 0.25]],
    [[Notes.A5, 0.5], [Notes.A5, 0.5], [Notes.G5, 0.5], [Notes.F5, 0.5],
        [Notes.E5, 0.5], [Notes.C5, 0.25], [Notes.C5, 0.25 + 0.25], [Notes.C5, 0.25], [Notes.C5, 0.25], [Notes.E5, 0.25]],
    [[Notes.G5, 0.25], [Notes.G5, 0.25], [Notes.G5, 0.25], [Notes.G5, 0.25],
        [Notes.F5, 0.25], [Notes.F5, 0.25], [Notes.E5, 0.25], [Notes.E5, 0.25],
        [Notes.F5, 0.5], [Notes.D5, 0.25], [Notes.D5, 0.25 + 0.75], [Notes.D5, 0.25]],
    [[Notes.A5, 0.5], [Notes.A5, 0.5], [Notes.G5, 0.5], [Notes.F5, 0.5],
        [Notes.E5, 0.5], [Notes.C5, 0.5],
        [Notes.C5, 0.25], [Notes.C5, 0.25], [Notes.C5, 0.25], [Notes.E5, 0.25]],
    [[Notes.G5, 0.25], [Notes.G5, 0.25], [Notes.G5, 0.25], [Notes.G5, 0.25], [Notes.F5, 0.5], [Notes.E5, 0.5],
        [Notes.F5, 0.5], [Notes.D5, 0.25], [Notes.D5, 0.25 + 1]]

]

Ievan_polkka = Score(Ievan_polkka_content, ["B"], names[2])


def get_data(score: Score):
    template = """
sound {name}[] = {{
    {content}
    {last}
}};
    """

    elem_template = "{{{:2d}, {:7d}}}, "
    last = "{00, 2000000}"
    content = ""
    for line in score.content:
        for elem in line:
            note = elem[0].value
            if any(char in elem[0].name for char in score.go_flat):
                note -= 1
            content += elem_template.format(note, int(elem[1] * FREQ_DIV * 0.8))
        content += "\n    "
    content = content[:-len("\n    ")]

    result = template.format(
        name = score.name, 
        content = content,
        last = last
    )
    print(result)

get_data(anthem)

get_data(BWV846)

get_data(Ievan_polkka)
