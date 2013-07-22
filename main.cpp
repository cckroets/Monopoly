//
//  main.cpp
//  MonopolyDistribution
//
//  Created by Curtis Kroetsch on 2013-07-17.
//  Copyright (c) 2013 Curtis Kroetsch. All rights reserved.
//

#include <iostream>
#include "Deck.h"
#include "Board.h"


int main(int argc, const char * argv[]) {
    
    if (argc != 3) {
        std::cout << "Please enter #PLAYERS and #TURNS as arguments" << std::endl;
        return 1;
    }
        
    int num_players = atoi(argv[1]);
    int rounds = atoi(argv[2]);

    
    Game G(num_players);
    
    G.play(rounds);
    
    G.print();

    return 0;
}

