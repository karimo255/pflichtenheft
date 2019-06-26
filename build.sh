!/bin/bash

gcc main.c src/core/game.c src/core/view.c src/services/user_service.c src/services/score_service.c libs/sqlite3.c  -lpthread -ldl -o app  && ./app
