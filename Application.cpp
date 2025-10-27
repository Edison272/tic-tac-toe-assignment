#include "Application.h"
#include "imgui/imgui.h"
#include "classes/TicTacToe.h"

namespace ClassGame {
        //
        // our global variables
        //
        TicTacToe *game = nullptr;
        bool gameOver = false;
        int gameWinner = -1;

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            game = new TicTacToe();
            game->setUpBoard();
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
                ImGui::DockSpaceOverViewport();

                //ImGui::ShowDemoWindow();

                if (!game) return;
                if (!game->getCurrentPlayer()) return;
                
                // draw settings menu
                ImGui::Begin("Settings");
                ImGui::Text("Current Player Number: %d", game->getCurrentPlayer()->playerNumber());
                ImGui::Text("Current Board State: %s", game->stateString().c_str());

                ImGui::SeparatorText("AI Settings");
                std::string toggle_ai_label = "Toggle AI | Currently " + std::string(game->_gameOptions.AIPlaying ? "On" : "Off");
                if (ImGui::Button(toggle_ai_label.c_str())) {
                    game->_gameOptions.AIPlaying = !game->_gameOptions.AIPlaying;
                    game->setAIPlayer(1);
                }

                if (gameOver) {
                    ImGui::SeparatorText("End of Game");
                    ImGui::Text("Game Over!");
                    if (gameWinner == -1) {
                        ImGui::Text("===== Draw =====");
                    } else {
                        ImGui::Text("=== Winner: %d ===", gameWinner);
                    }
                    if (ImGui::Button("Reset Game")) {
                        game->stopGame();
                        game->setUpBoard();
                        gameOver = false;
                        gameWinner = -1;
                        game->_gameOptions.AIPlaying = false;
                    }
                }
                ImGui::End();

                // draw game
                ImGui::Begin("GameWindow");
                game->drawFrame();
                ImGui::End();
        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
            Player *winner = game->checkForWinner();
            if (winner)
            {
                gameOver = true;
                gameWinner = winner->playerNumber();
            }
            if (game->checkForDraw()) {
                gameOver = true;
                gameWinner = -1;
            }
        }
}
