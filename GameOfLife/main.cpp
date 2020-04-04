//
//  GameOfLife.cpp
//  GameOfLife
//
//  Created by Timothy Mwiti on 2/3/20.
//  Copyright © 2020 tim. All rights reserved.
//

#include <stdio.h>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#define WIDTH 100
#define HEIGHT 100

class GameOfLife : public olc::PixelGameEngine {
public:
    GameOfLife() {
        sAppName = "Game of Life";
    }
private:
    bool curGen[WIDTH][HEIGHT];
    bool nextGen[WIDTH][HEIGHT];
    bool useCurToUpdate;
    bool patternDrawingComplete;



public:
    bool OnUserCreate() override {
        useCurToUpdate = true;
        patternDrawingComplete = false;
        Clear(olc::WHITE);
        
        for(int i = 0; i < WIDTH; i++){
            for(int j = 0; j < HEIGHT; j++){
                curGen[i][j] = false;
            }
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if(GetKey(olc::Key::A).bPressed){
            patternDrawingComplete = true;
        }else if(GetKey(olc::Key::Q).bPressed){ // Exit Game
            return false;
        }

        if(! patternDrawingComplete){
            getUserPatternInput();
        }else{
            // Clear(olc::WHITE);
            std::chrono::milliseconds timespan(20);
            std::this_thread::sleep_for(timespan);
            for (int x = 0; x < ScreenWidth(); x++){
                for (int y = 0; y < ScreenHeight(); y++){
                    if(useCurToUpdate){
                        nextGen[x][y] = getCellState(x, y, curGen[x][y], true);
                    }else{
                        curGen[x][y] = getCellState(x, y, nextGen[x][y], false);;
                    }
                }
            }

            if(useCurToUpdate){
                drawPattern(nextGen);
            }else{
                drawPattern(curGen);
            }
            
            useCurToUpdate = !useCurToUpdate;
        }
        return true;
    }
    
    void getUserPatternInput(){
        if(GetMouse(0).bHeld){
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();
            curGen[mouseX][mouseY] = true;
        }


        for(int x = 0; x < WIDTH; x++){
            for(int y = 0; y < HEIGHT; y++){
                if(curGen[x][y]){
                    Draw(x, y, olc::BLACK);
                }
            }
        }
    }
    
    void drawPattern(bool gen[WIDTH][HEIGHT]){
        Clear(olc::WHITE);
        for(int x = 0; x < WIDTH; x++){
            for(int y = 0; y < HEIGHT; y++){
                if(gen[x][y]){
                    Draw(x, y, olc::BLACK);
                }
            }
        }
    }

    bool getCellState(int w, int h, bool curState, bool useCur){
        
        int aliveCount = 0;
        if(useCur){
            for(int i = w-1; i <= w+1; i++){
                for(int j = h-1; j <= h+1; j++){
                    if(i == w && j == h){
                        continue;
                    }
                    if(i > -1 && i < WIDTH-1 && j > -1 && j < HEIGHT-1 && curGen[i][j]){
                        aliveCount++;
                    }
                }
            }
        }else{
            for(int i = w-1; i <= w+1; i++){
                for(int j = h-1; j <= h+1; j++){
                    if(i == w && j == h){
                        continue;
                    }
                    if(i > -1 && i < WIDTH-1 && j > -1 && j < HEIGHT-1 && nextGen[i][j]){
                        aliveCount++;
                    }
                }
            }
        }

        if(curState){
            if(aliveCount > 1 && aliveCount < 4){
                return true;
            }
            return false;
        }
        
        if (aliveCount == 3) {
            return true;
        }
        return false;
    }

};


int main(int argc, char const *argv[]) {
    GameOfLife demo;
    if (demo.Construct(WIDTH, HEIGHT, 125, 125))
        demo.Start();

    return 0;
}
