gclh=programs/functions/game_clues/game_clues.h
gclc=programs/functions/game_clues/game_clues.cpp
gmah=programs/functions/game_math/game_math.h
gmac=programs/functions/game_math/game_math.cpp
gmeh=programs/functions/game_mechanics/game_mechanics.h
gmec=programs/functions/game_mechanics/game_mechanics.cpp
gstrh=programs/functions/game_structure/game_structure.h
gstrc=programs/functions/game_structure/game_structure.cpp
gsynh=programs/functions/game_syntax/game_syntax.h
gsync=programs/functions/game_syntax/game_syntax.cpp
gfh=programs/functions/gamefile/gamefile.h
gfc=programs/functions/gamefile/gamefile.cpp
gsh=programs/functions/gameseed/gameseed.h
gsc=programs/functions/gameseed/gameseed.cpp
geh=programs/functions/generate_expression/generate_expression.h
gec=programs/functions/generate_expression/generate_expression.cpp
lh=programs/functions/leaderboard/leaderboard.h
lc=programs/functions/leaderboard/leaderboard.cpp
uih=programs/functions/user_input/user_input.h
uic=programs/functions/user_input/user_input.cpp
mainc=programs/main.cpp
gclo=game_clues.o
gmao=game_math.o
gmeo=game_mechanics.o
gstro=game_structure.o
gsyno=game_syntax.o
gfo=gamefile.o
gso=gameseed.o
geo=generate_expression.o
lo=leaderboard.o
uio=user_input.o
maino=main.o

game_clues.o: $(gclc) $(gclh)
	g++ -c $(gclc)
game_math.o: $(gmac) $(gmah)
	g++ -c $(gmac)
game_mechanics.o: $(gmec) $(gmeh)
	g++ -c $(gmec)
game_structure.o: $(gstrc) $(gstrh)
	g++ -c $(gstrc)
game_syntax.o: $(gsync) $(gsynh)
	g++ -c $(gsync)
gamefile.o: $(gfc) $(gfh)
	g++ -c $(gfc)
gameseed.o: $(gsc) $(gsh)
	g++ -c $(gsc)
generate_expression.o: $(gec) $(geh)
	g++ -c $(gec)
leaderboard.o: $(lc) $(lh)
	g++ -c $(lc)
user_input.o: $(uic) $(uih)
	g++ -c $(uic)
main.o: $(mainc) $(gstrh)
	g++ -c $(mainc)
main: $(maino) $(gclo) $(gmao) $(gmeo) $(gstro) $(gsyno) $(gfo) $(gso) $(geo) $(lo) $(uio) 
	g++ -pedantic-errors -std=c++11 $(maino) $(gclo) $(gmao) $(gmeo) $(gstro) $(gsyno) $(gfo) $(gso) $(geo) $(lo) $(uio) -o $@
clean:
	rm -f $(gclo) $(gmao) $(gmeo) $(gstro) $(gsyno) $(gfo) $(gso) $(geo) $(lo) $(uio) $(maino) main
.PHONY: clean
