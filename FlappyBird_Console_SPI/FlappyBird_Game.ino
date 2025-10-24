#define BirdGroundPlane 56



#define Low_pipe_HIGH 25 
#define Low_pipe_LOW 60 
#define Middle_pipe_HIGH 15 
#define Middle_pipe_LOW 52 
#define Top_pipe_HIGH 5 
#define Top_pipe_LOW 42 

int8_t Pipe_Collision_Points [2][3] = {
	{Low_pipe_HIGH, Middle_pipe_HIGH, Top_pipe_HIGH,},
	{Low_pipe_LOW, Middle_pipe_LOW, Top_pipe_LOW,}
	};


void FBirdGame(int8_t EEPROM_ADDR , int8_t Difficulty) {

  StartBirdGame:

	WaitingScreen();

	float BirdU = 0.0;
	float BGravity = 0.0;
	int birdY = 20;
	int NumOfFlappedFrames = 0;
	bool BirdFlapp = false;
	bool GameOVER = false;
	uint16_t PipeCounter = 0;
	uint16_t BestScore = 0;
	bool PipeIsDeadly = false;
	bool SpeedIncreaseText = false;
	int8_t SpeedStep = 10;
	int8_t GameSpeed = 30;
	int8_t EnemyStep = 1;
	int8_t GspeedInc = 1;
	int8_t increasePipeCounter = 0;



	int8_t oldPipePos = 128;
	int8_t oldPipeType = 0;
	int8_t newPipePos = 128;
	int8_t newPipeType = random(0, 3);

	int8_t firstBackgroundPos = 64;
	int8_t secondBackgroundPos = 64;
	int8_t thirdBackgroundPos = 128;

	switch (Difficulty) {
		case 0: GameSpeed = 30; EnemyStep = 1; GspeedInc = 3; BGravity = 0.060f;
		break;

		case 1: GameSpeed = 20; EnemyStep = 2; GspeedInc = 2; BGravity = 0.080f;
		break;

		case 2:	GameSpeed = 10; EnemyStep = 2; GspeedInc = 1; BGravity = 0.100f;
		break;
	}

	EEPROM.get(EEPROM_ADDR, BestScore);



  while (1) {
  

    /*  ---------------- USER INPUT ----------------  */

		if (main_button.isClick()){
			BirdU = -0.8;
			birdY -= 2;
			NumOfFlappedFrames = 2000;

		}







    /*  ---------------- GAME PROCESSING ----------------  */



		switch (PipeCounter) {
			case 10: SpeedIncreaseText = true; break;
			case 11: SpeedStep = 10; break;
			case 20: SpeedIncreaseText = true; break;
			case 21: SpeedStep = 20; break;
			case 40: SpeedIncreaseText = true; break;
			case 41: SpeedStep = 10; break;
			case 50: SpeedIncreaseText = true; break;
			case 51: SpeedStep = 30; break;
			case 80: SpeedIncreaseText = true; break;
			case 81: SpeedStep = 20; break;
			case 100: SpeedIncreaseText = true; break;
			case 101: SpeedStep = 50; break;
			case 150: SpeedIncreaseText = true; break;
		}

    if (increasePipeCounter / SpeedStep >= 1) {
        GameSpeed -= GspeedInc;
				increasePipeCounter = 0;     
    }


		static uint32_t PipeTimer = millis();
		if(millis() - PipeTimer >= GameSpeed){
			PipeTimer = millis();
			newPipePos -= EnemyStep;

			if(newPipePos <= 12){
				oldPipePos = newPipePos;
				oldPipeType = newPipeType;
				newPipePos = 128;
				newPipeType = random(0, 3);
				PipeCounter += 1;
				increasePipeCounter += 1;
			}
			if(oldPipePos >= -24){
				oldPipePos -= EnemyStep;
				if (oldPipePos <= 2) {
					PipeIsDeadly = false;
				}
				if (oldPipePos > 2) PipeIsDeadly = true;
			}
		}

		static uint32_t BackgroundTimer = millis();
		if (millis() - BackgroundTimer >= 25) {
			BackgroundTimer = millis();
			thirdBackgroundPos -= 1;
			if (thirdBackgroundPos <= 64) {
				secondBackgroundPos = thirdBackgroundPos;
				thirdBackgroundPos = 128;
			}
			secondBackgroundPos -= 1;
			if (secondBackgroundPos <= 0) {
				firstBackgroundPos = secondBackgroundPos;
			}
			if (firstBackgroundPos >= -64) {
				firstBackgroundPos -= 1;
			}
		}



		static uint32_t FlapTimer = millis();
		if(millis() - FlapTimer >= 400){
			if (NumOfFlappedFrames != 0) {
				BirdFlapp = true;
				NumOfFlappedFrames -= 1;
			}
			else {
				BirdFlapp = false;
			}
		}

		static uint32_t BridGravityTimer = millis();
		if(millis() - BridGravityTimer >= 10){
			BridGravityTimer = millis();

			BirdU += (float)BGravity;
			birdY += (float)BirdU;



		}




    


    /*  ---------------- DRAWING ----------------  */

    static uint32_t drawTimer = millis();
    
    if (millis() - drawTimer >= (1000 / Game_FPS)) {
      
      drawTimer = millis();
      oled.clear();
			oled.setScale(1);
			oled.setCursorXY(0, 2); oled.print("Sc:"); oled.print(PipeCounter);
			if (SpeedIncreaseText) {
				oled.setCursorXY(0, 55);
				oled.print("+ Speed");
				SpeedIncreaseText = false;
			}
			oled.line(0, 0, 127, 0); 
			oled.line(0, 63, 127, 63);  

			switch (oldPipeType) {
				case 0: oled.drawBitmap(oldPipePos, 0, bitmap__Low_pipe, 16, 64);
				break;
				case 1: oled.drawBitmap(oldPipePos, 0, bitmap__Middle_pipe, 16, 64);
				break;
				case 2: oled.drawBitmap(oldPipePos, 0, bitmap__Top_pipe, 16, 64);
				break;
			}

			switch (newPipeType) {
				case 0: oled.drawBitmap(newPipePos, 0, bitmap__Low_pipe, 16, 64);
				break;
				case 1: oled.drawBitmap(newPipePos, 0, bitmap__Middle_pipe, 16, 64);
				break;
				case 2: oled.drawBitmap(newPipePos, 0, bitmap__Top_pipe, 16, 64);
				break;
			}
			
			oled.drawBitmap(firstBackgroundPos, 0, bitmap__Background, 64, 64);
			oled.drawBitmap(secondBackgroundPos, 0, bitmap__Background, 64, 64);
			oled.drawBitmap(thirdBackgroundPos, 0, bitmap__Background, 64, 64);

			if (birdY >= BirdGroundPlane or birdY <= -8) {
				GameOVER = true;
			}
			if (PipeCounter > 1) {
				if (birdY   < Pipe_Collision_Points[0][oldPipeType] && PipeIsDeadly or birdY + 16 > Pipe_Collision_Points[1][oldPipeType] && PipeIsDeadly) {
					GameOVER = true;
				}
			}

			if(GameOVER){
				WaitingScreen();
				if(PipeCounter > BestScore){
					EEPROM.put(EEPROM_ADDR, PipeCounter);
          BestScore = PipeCounter;
					delay(500);
					oled.clear();
					oled.roundRect(0, 1, 127, 62, OLED_STROKE);
					oled.setScale(2);
					oled.setCursor(4, 1); oled.print(" NEW HIGH ");
					oled.setCursor(4, 3); oled.print("  SCORE!  ");
					oled.setCursor(40,5); oled.print(BestScore);
					oled.update();
					delay(500);
					while(1){
						if(main_button.isClick()) goto EndScreen;
					}
				}
				EndScreen:
				delay(500);
				oled.clear();
				oled.roundRect(0, 1, 127, 62, OLED_STROKE);
				oled.setScale(2);
				oled.setCursor(7, 1);
				oled.print(F("GAME OVER!"));
				oled.setScale(1);
				oled.setCursor(7, 4); oled.print("Score : "); oled.print(PipeCounter);
				oled.setCursor(7, 6); oled.print("Best Score : "); oled.print(BestScore);
				oled.update();
				delay(500);
				while (1) {
					if (main_button.isClick()) goto StartBirdGame;
          if (main_button.isHold()) loop();
				}
			}

      oled.drawBitmap(0, birdY, BirdFlapp ? bitmap__Bird_Flapped : bitmap__Bird_Normal, 16, 16);
      oled.update();
    }




  }

}