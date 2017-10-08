'''
Script to convert Othello openings catalog from
http://www.othello.nl/content/anim/openings.txt
to a .csv file, to be used as an openings book.
'''


def make_index(string, t3=False):
    index = ''
    string = string.lower()
    moves = [string[i:i+2] for i in range(0, len(string), 2)]

    if t3:
        moves[0:3] = moves[0:3][::-1]

    for move in moves:
        if move[0] == 'a':
            col = 0
        elif move[0] == 'b':
            col = 1
        elif move[0] == 'c':
            col = 2
        elif move[0] == 'd':
            col = 3
        elif move[0] == 'e':
            col = 4
        elif move[0] == 'f':
            col = 5
        elif move[0] == 'g':
            col = 6
        elif move[0] == 'h':
            col = 7

        if move[1] == '1':
            row = 0
        elif move[1] == '2':
            row = 1
        elif move[1] == '3':
            row = 2
        elif move[1] == '4':
            row = 3
        elif move[1] == '5':
            row = 4
        elif move[1] == '6':
            row = 5
        elif move[1] == '7':
            row = 6
        elif move[1] == '8':
            row = 7

        index += str(8*row + col) + ','

    return index[:-1] + '\n'


with open('openings.dat', 'r') as infile:
    with open('openings.csv', 'w') as outfile:
        for line in infile:
            splut = line.split()
            outfile.write(make_index(splut[0]))
            if (splut[-1] == '(t3)'):
                outfile.write(make_index(splut[0], True))
