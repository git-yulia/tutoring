# Analysis of Algorithms
# Practice Assignment 1
# Author: Julia Maliauka
# Quiz 6 - The Gale-Shapley Algorithm 

"""
Given two sets of size n
    B = {b1, b2, ... , bn} and G = {g1, g2, ... , gn}
Let <i denote the ranking of boy b(i) to a g.
g(i) < g(i) denotes favorites of girls to that specific b. 

Blocking pair definition - 
b is involved with g'
b' is involved with g 
but g prefers b and b prefers g than their current matches. 

A matching M is stable if it contains no blocking pairs. 
"""

stage = 1 

n = 4    # hardcoded number of people in each group 
top = 0
bottom = (n - 1)

class Boy():
    def __init__(self, name, preferences):
        self.name = name 
        self.preferences = preferences
        self.partner = None

    def elope(self, partner):
        self.partner = partner

class Girl():
    def __init__(self, name, preferences):
        self.name = name 
        self.preferences = preferences
        self.partner = None
        self.isEngaged = False

    def elope(self, partner):
        self.partner = partner
        self.isEngaged = True

b1 = Boy("Johnny", [0,1,3,2])
b2 = Boy("Jesse",  [1,2,3,0])
b3 = Boy("Thomas", [3,0,2,1])
b4 = Boy("Connor", [3,2,1,0])

g1 = Girl("Yulia",    [0,1,3,2])
g2 = Girl("Kelly",    [1,0,2,3])
g3 = Girl("Margaret", [0,1,3,2])
g4 = Girl("Avery",    [0,2,3,1])

B = [b1, b2, b3, b4]
G = [g1, g2, g3, g4]
# initialize empty set of matches 
Matches = [[None,None],[None,None],[None,None],[None,None]] 

# Finally, the algorithm 
# Stage 1 - Initialization. The first proposal is made...
# b1 chooses his top g and this pairing is added to Matches
b1.elope(G[b1.preferences[top]])
b1.partner.elope(b1) 
newleyweds = 0
Matches[newleyweds] = [b1,b1.partner]

# new variable, boy, to describe the next hopeful suitor 
boy = b1
while(stage < n):     # why n - 1? 
    boy = B[stage] 

    for girl in boy.preferences:
        if (G[girl].isEngaged == False):
            boy.elope(G[boy.preferences[girl]])
            newleyweds = newleyweds + 1            

            print("Indexing ", newleyweds, "boy is ", boy.name, " ", boy.partner.name)
            Matches[newleyweds] = [boy,boy.partner]
            boy.partner.elope(boy)

    print("...")
    stage = stage + 1

for i in range(n):
    print(Matches[i][0].name, " <3 ", Matches[i][1].name)