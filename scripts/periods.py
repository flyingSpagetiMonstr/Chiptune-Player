
note_names = [
    "O", # -10
    "C2", "CD2", "D2", "DE2", "E2", "F2", "FG2", "G2", "GA2", "A2", "AB2", "B2",
    "C3", "CD3", "D3", "DE3", "E3", "F3", "FG3", "G3", "GA3", "A3", "AB3", "B3",
    "C4", "CD4", "D4", "DE4", "E4", "F4", "FG4", "G4", "GA4", "A4", "AB4", "B4",
    "C5", "CD5", "D5", "DE5", "E5", "F5", "FG5", "G5", "GA5", "A5", "AB5", "B5"
]

BASE = (2000 * 2)

def get_base(freq_div):
    # print("freq_div: ", freq_div)
    unit = 1 / freq_div
    # print("unit: ", unit)

    A440_period_f = 440
    A440_period = 1 / A440_period_f
    A440_period_n = A440_period / unit

    # A440_period_n = BASE

    return A440_period_n

def get_table(div):
    notes = [ -note_names.index("A4") + i for i in range(len(note_names))] # A -> 0
    base = get_base(div)
    # print("base: ", base)
    periods = [int(base / (2**(note/12))) for note in notes]
    
    # for i in range(len(periods)):
    #     print(note_names[i] + ": ", end='')
    #     print(periods[i])
    print(periods)

# print("###################################")
get_table(1000_000)
