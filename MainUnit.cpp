//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	srand(GetTickCount()); //use current time as seed for random generator
	posX = 0;
	posY = 0;

	nextPieceBuffer = new TBitmap();
	nextPieceBuffer->Width = PaintBoxNextPiece->Width;
	nextPieceBuffer->Height = PaintBoxNextPiece->Height;

	scoreBuffer = new TBitmap();
	scoreBuffer->Width = ScorePaintBox->Width;
	scoreBuffer->Height = ScorePaintBox->Height;

	gameBoardBuffer = new TBitmap();
	gameBoardBuffer->Width = GameBoardPaintBox->Width;
	gameBoardBuffer->Height = GameBoardPaintBox->Height;

	levelBuffer = new TBitmap();
	levelBuffer->Width = LevelPaintBox->Width;
	levelBuffer->Height = LevelPaintBox->Height;

	infoBuffer = new TBitmap();
	infoBuffer->Width = InfoPaintBox->Width;
	infoBuffer->Height = InfoPaintBox->Height;
}
//---------------------------------------------------------------------------

void TMainForm::InitializeGrid()
{
	for (int y = 0; y < GRID_HEIGHT; y++)
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			grid[y][x] = 0; // 0 means the cell is empty
            ColorGrid[x][y] = clWhite;
        }
    }
}
//---------------------------------------------------------------------------

void TMainForm::SetCell(int x, int y, int value)
{
	if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT)
	{
		grid[y][x] = value;
    }
}
//---------------------------------------------------------------------------

int TMainForm::GetCell(int x, int y)
{
	if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT)
	{
        return grid[y][x];
    }
    return -1; // Return -1 if out of bounds
}
//---------------------------------------------------------------------------

void TMainForm::DrawBlock(TCanvas* Canvas, int x, int y, TColor color)
{
    // Calculate the position of the block on the grid
    int drawX = x * BLOCK_SIZE;
    int drawY = y * BLOCK_SIZE;

    // Fill the block with the specified color
    Canvas->Brush->Color = color;
    Canvas->FillRect(Rect(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE));

    // Draw the interior lighter border (slightly inset)
    TColor lighterShade = TColor(RGB(169, 169, 169)); // Light shade of black (dark gray)
    Canvas->Pen->Color = lighterShade;
    Canvas->Pen->Width = 1;
    Canvas->Rectangle(Rect(drawX + 1, drawY + 1, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1));

    // Draw the outer solid black border
    Canvas->Pen->Color = clBlack;
    Canvas->Pen->Width = 1;
	Canvas->Rectangle(Rect(drawX, drawY, drawX + BLOCK_SIZE, drawY + BLOCK_SIZE));
}
//---------------------------------------------------------------------------

void TMainForm::RenderGrid()
{
    gameBoardBuffer->Canvas->Brush->Color = clWhite;
    gameBoardBuffer->Canvas->FillRect(Rect(0, 0, gameBoardBuffer->Width, gameBoardBuffer->Height));

	for (int y = 0; y < GRID_HEIGHT; y++)
	{
		for (int x = 0; x < GRID_WIDTH; x++)
		{
			if (grid[y][x] > 0 && grid[y][x] != clWhite)
			{
				DrawBlock(gameBoardBuffer->Canvas, x, y, ColorGrid[y][x]);
			}
			else
			{
                gameBoardBuffer->Canvas->Brush->Color = clWhite;
                gameBoardBuffer->Canvas->FillRect(Rect(x * BLOCK_SIZE, y * BLOCK_SIZE,
                                                       (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE));
                gameBoardBuffer->Canvas->Brush->Color = clGray;
                gameBoardBuffer->Canvas->FrameRect(Rect(x * BLOCK_SIZE, y * BLOCK_SIZE,
                                                        (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE));
            }
        }
	}
}

//---------------------------------------------------------------------------

void TMainForm::RenderTetrimino()
{
	for (int y = 0; y < TETRIMINO_SIZE; y++)
	{
		for (int x = 0; x < TETRIMINO_SIZE; x++)
		{
			if (tetrimino.grid[y][x] > 0)
			{
                DrawBlock(gameBoardBuffer->Canvas, posX + x, posY + y, tetrimino.color);
            }
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::WMEraseBkgnd(TMessage &Message)
{

}
//---------------------------------------------------------------------------

bool TMainForm::CanMove(const Tetrimino& tetrimino, int newX, int newY)
{
	for (int y = 0; y < TETRIMINO_SIZE; y++)
	{
		for (int x = 0; x < TETRIMINO_SIZE; x++)
		{
			if (tetrimino.grid[y][x] > 0)
			{ // Only check filled cells

                // Calculate the corresponding position on the game grid
                int gridX = newX + x;
                int gridY = newY + y;

                // Check if this position is outside the grid boundaries
				if (gridX < 0 || gridX >= GRID_WIDTH || gridY < 0 || gridY >= GRID_HEIGHT)
				{
                    return false; // Out of bounds
                }

                // Check if this cell on the game grid is already occupied
				if (grid[gridY][gridX] > 0)
				{
                    return false; // Collision with another piece
                }
            }
		}
	}
	return true; // No collision, movement is possible

}
//---------------------------------------------------------------------------

void TMainForm::LockTetrimino()
{
	if (!CanMove(tetrimino, posX, posY))
	{
		InitializeGrid(); // Game over or reset
		return;
	}

	for (int y = 0; y < TETRIMINO_SIZE; y++)
	{
		for (int x = 0; x < TETRIMINO_SIZE; x++)
		{
			if (tetrimino.grid[y][x] > 0)
			{
                int gridX = posX + x;
                int gridY = posY + y;

				if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT)
				{
					grid[gridY][gridX] = 1;
                    ColorGrid[gridY][gridX] = tetrimino.color;
                }
			}
        }
	}
    // Copy the current Tetrimino into the grid
	for (int y = 0; y < TETRIMINO_SIZE; y++)
	{
		for (int x = 0; x < TETRIMINO_SIZE; x++)
		{
			if (tetrimino.grid[y][x] > 0)
			{
				ColorGrid[posY + y][posX + x] = tetrimino.color; // Store the Tetrimino's color
            }
        }
	}
    ClearLines();
	SpawnNewTetrimino();
}
//---------------------------------------------------------------------------

void TMainForm::ClearLines()
{
	int linesCleared = 0;

	for (int y = GRID_HEIGHT - 1; y >= 0; y--)
	{
        bool isFullLine = true;

		for (int x = 0; x < GRID_WIDTH; x++)
		{
            if (grid[y][x] == 0) { // Check occupancy
                isFullLine = false;
                break;
            }
        }

		if (isFullLine)
		{
            linesCleared++;

            // Shift rows above down
			for (int yy = y; yy > 0; yy--)
			{
				for (int x = 0; x < GRID_WIDTH; x++)
				{
                    grid[yy][x] = grid[yy - 1][x];
                    ColorGrid[yy][x] = ColorGrid[yy - 1][x];
                }
            }

            // Clear the top row
			for (int x = 0; x < GRID_WIDTH; x++)
			{
                grid[0][x] = 0;
                ColorGrid[0][x] = clWhite;
            }

            y++; // Recheck the same row after shifting
        }
	}

	lineCount += linesCleared;
	if(lineCount > 9)
	{
		level++;
        RenderLevel();
		lineCount = 0;
        Timer -= 50;
    }

	switch (linesCleared)
	{
	case 1:
		score += 500;
		break;
	case 2:
		score += 2200;
		break;
	case 3:
		score += 3500;
		break;
	case 4:
		score += 4800;
		break;
	}



	RenderScore();
}
//---------------------------------------------------------------------------

void TMainForm::SpawnNewTetrimino()
{
	// Set the next piece as the active piece and generate a new next piece
	tetrimino = nextTetrimino;
	posX = GRID_WIDTH / 2 - 1;
	posY = 0;
	nextTetrimino = shapes[rand() % 7];
	RenderNextTetrimino();

	posX = GRID_WIDTH / 2 - 1; // Center the new Tetrimino horizontally
    posY = 0;                   // Start at the top of the grid

    // Check if the new Tetrimino can spawn; if not, handle game over
	if (!CanMove(tetrimino, posX, posY))
	{
		InitializeGrid(); // Reset the grid to simulate a game over
	}
}
//---------------------------------------------------------------------------

Tetrimino TMainForm::RotateTetrimino(const Tetrimino& tetrimino)
{
	Tetrimino rotated = tetrimino;

	for (int y = 0; y < TETRIMINO_SIZE; y++)
	{
		for (int x = 0; x < TETRIMINO_SIZE; x++)
		{
            rotated.grid[x][TETRIMINO_SIZE - 1 - y] = tetrimino.grid[y][x];
        }
    }

	return rotated;
}
//---------------------------------------------------------------------------

void TMainForm::RotateCurrentTetrimino()
{
    Tetrimino rotated = RotateTetrimino(tetrimino);

	// Check if the rotated Tetrimino can fit in the current position
	if (CanMove(rotated, posX, posY))
	{
		tetrimino = rotated; // Apply the rotation if there’s no collision
	}
}
//---------------------------------------------------------------------------

void TMainForm::RenderNextTetrimino()
{
    // Clear the off-screen buffer
    nextPieceBuffer->Canvas->Brush->Color = clWhite;
    nextPieceBuffer->Canvas->FillRect(Rect(0, 0, nextPieceBuffer->Width, nextPieceBuffer->Height));

    // Draw the border on the buffer
    TRect borderRect(5, 5, nextPieceBuffer->Width - 5, nextPieceBuffer->Height - 5);
    nextPieceBuffer->Canvas->Brush->Style = bsClear;
    nextPieceBuffer->Canvas->Pen->Color = clBlack;
    nextPieceBuffer->Canvas->Pen->Width = 2;
    nextPieceBuffer->Canvas->Rectangle(borderRect);

    // Draw "Next Piece" label
    nextPieceBuffer->Canvas->Font->Size = 10;
    nextPieceBuffer->Canvas->Font->Style = TFontStyles() << fsBold;
    nextPieceBuffer->Canvas->TextOut(10, 5, "Next Piece");

    // Center the Tetrimino preview in the buffer
    int blockSize = BLOCK_SIZE / 2; // Smaller blocks for the preview
    int offsetX = (borderRect.Width() - TETRIMINO_SIZE * blockSize) / 2 + borderRect.Left;
	int offsetY = borderRect.Top + 35;

    // Render the next Tetrimino with borders
	for (int y = 0; y < TETRIMINO_SIZE; y++)
	{
		for (int x = 0; x < TETRIMINO_SIZE; x++)
		{
			if (nextTetrimino.grid[y][x] > 0)
			{
				int blockStartX = 30 + x * blockSize * 2;
				int blockStartY = 30 + y * blockSize * 2;
				int blockEndX = blockStartX + blockSize * 2;
                int blockEndY = blockStartY + blockSize * 2;

                // Fill the block with the Tetrimino color
                nextPieceBuffer->Canvas->Brush->Color = nextTetrimino.color;
                nextPieceBuffer->Canvas->FillRect(Rect(blockStartX, blockStartY, blockEndX, blockEndY));

                // Draw the interior lighter border (slightly inset)
                TColor lighterShade = TColor(RGB(169, 169, 169)); // Light shade of black
                nextPieceBuffer->Canvas->Pen->Color = lighterShade;
                nextPieceBuffer->Canvas->Rectangle(Rect(blockStartX + 1, blockStartY + 1, blockEndX - 1, blockEndY - 1));

                // Draw the outer solid black border
                nextPieceBuffer->Canvas->Pen->Color = clBlack;
                nextPieceBuffer->Canvas->Rectangle(Rect(blockStartX, blockStartY, blockEndX, blockEndY));
            }
        }
    }

    // Copy the buffer to the paint box to display it
	PaintBoxNextPiece->Canvas->Draw(0, 0, nextPieceBuffer);
}
//---------------------------------------------------------------------------

void TMainForm::RenderLevel()
{
    // Clear the LevelPaintBox area
	levelBuffer->Canvas->Brush->Color = clWhite;
	levelBuffer->Canvas->FillRect(Rect(0, 0, LevelPaintBox->Width, LevelPaintBox->Height));

    // Draw the border around the LevelPaintBox
    TRect borderRect(2, 2, LevelPaintBox->Width - 2, LevelPaintBox->Height - 2);
	levelBuffer->Canvas->Brush->Style = bsClear; // Make sure the border is only an outline
	levelBuffer->Canvas->Pen->Color = clBlack;   // Border color
	levelBuffer->Canvas->Pen->Width = 2;         // Set a thicker border width
	levelBuffer->Canvas->Rectangle(borderRect);

    // Draw "Level" label at the top of the box
	levelBuffer->Canvas->Font->Size = 10;
	levelBuffer->Canvas->Font->Style = TFontStyles() << fsBold;
	levelBuffer->Canvas->TextOut(10, 5, "Level");

    // Draw the current level number centered below the label
	levelBuffer->Canvas->Font->Size = 16;
	levelBuffer->Canvas->Font->Style = TFontStyles();
    int textX = LevelPaintBox->Width / 2 - 10; // Adjust to center the text horizontally
    int textY = LevelPaintBox->Height / 2;      // Adjust to position the text vertically
	levelBuffer->Canvas->TextOut(textX, textY, IntToStr(level));
	LevelPaintBox->Canvas->Draw(0, 0, levelBuffer);
}
//---------------------------------------------------------------------------

void TMainForm::RenderInfoBox()
{
	infoBuffer->Canvas->Brush->Color = clWhite;
	infoBuffer->Canvas->FillRect(Rect(0, 0, InfoPaintBox->Width, InfoPaintBox->Height));

	// Draw the border around the LevelPaintBox
	TRect borderRect(2, 2, InfoPaintBox->Width - 2, InfoPaintBox->Height - 2);
	infoBuffer->Canvas->Brush->Style = bsClear; // Make sure the border is only an outline
	infoBuffer->Canvas->Pen->Color = clBlack;   // Border color
	infoBuffer->Canvas->Pen->Width = 2;         // Set a thicker border width
	infoBuffer->Canvas->Rectangle(borderRect);

	// Draw text in box
	infoBuffer->Canvas->Font->Size = 10;
	infoBuffer->Canvas->Font->Style = TFontStyles();
	infoBuffer->Canvas->TextOut(10, 5, "Press 'n' to start a new game.");

	InfoPaintBox->Canvas->Draw(0, 0, infoBuffer);
}
//---------------------------------------------------------------------------

void TMainForm::RenderScore()
{
    // Clear the score buffer
	scoreBuffer->Canvas->Brush->Color = clWhite;
	scoreBuffer->Canvas->FillRect(Rect(0, 0, scoreBuffer->Width, scoreBuffer->Height));

    // Draw the border around the score display
    TRect borderRect(2, 2, scoreBuffer->Width - 2, scoreBuffer->Height - 2);
    scoreBuffer->Canvas->Brush->Style = bsClear;
    scoreBuffer->Canvas->Pen->Color = clBlack;
    scoreBuffer->Canvas->Pen->Width = 2;
    scoreBuffer->Canvas->Rectangle(borderRect);

    // Draw "Score" label
    scoreBuffer->Canvas->Font->Size = 10;
    scoreBuffer->Canvas->Font->Style = TFontStyles() << fsBold;
    scoreBuffer->Canvas->TextOut(10, 5, "Score");

    // Draw the current score centered below the label
    scoreBuffer->Canvas->Font->Size = 16;
    scoreBuffer->Canvas->Font->Style = TFontStyles();
    int textX = scoreBuffer->Width / 2 - 10;
    int textY = scoreBuffer->Height / 2;
	scoreBuffer->Canvas->TextOut(textX, textY, IntToStr(score));

    // Copy the buffer to the paint box
	ScorePaintBox->Canvas->Draw(0, 0, scoreBuffer);
	ScorePaintBox->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormPaint(TObject *Sender)
{
	// Render the game grid and active Tetrimino to the buffer
    RenderGrid();
	RenderTetrimino();

	// Copy the buffer to the GameBoardPaintBox to display it
	GameBoardPaintBox->Canvas->Draw(0, 0, gameBoardBuffer);
    PaintBoxNextPiece->Canvas->Draw(0,0, nextPieceBuffer);
	ScorePaintBox->Canvas->Draw(0, 0, scoreBuffer);
	LevelPaintBox->Canvas->Draw(0,0, levelBuffer);
    InfoPaintBox->Canvas->Draw(0, 0, infoBuffer);
}

//---------------------------------------------------------------------------

void __fastcall TMainForm::TimerTimer(TObject *Sender)
{
	// Only spawn a new Tetrimino if the current one can't move down
	if (!MoveTetriminoDown())
	{
		SpawnNewTetrimino();
	}
	Repaint(); // Force an immediate redraw
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
    InitializeGrid();
	nextTetrimino = shapes[rand() % 7]; // Set the first next piece

	offsetX = (GameBoardPaintBox->Width - GRID_WIDTH * BLOCK_SIZE) / 2;
	offsetY = (GameBoardPaintBox->Height - GRID_HEIGHT * BLOCK_SIZE) / 2;


	RenderLevel();
	RenderScore();
	RenderNextTetrimino();
	RenderInfoBox();

	MainForm->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	delete nextPieceBuffer;
	delete scoreBuffer;
	delete gameBoardBuffer;
    delete infoBuffer;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormResize(TObject *Sender)
{
    offsetX = (ClientWidth - GRID_WIDTH * BLOCK_SIZE) / 2;
    offsetY = (ClientHeight - GRID_HEIGHT * BLOCK_SIZE) / 2;
	Repaint(); // Redraw with the new offset
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch (Key)
	{
		case VK_LEFT:
			MoveTetriminoLeft();
			break;
		case VK_RIGHT:
			MoveTetriminoRight();
			break;
		case VK_DOWN:
			MoveTetriminoDown();
			break;
		case VK_UP: // Rotate the piece
			RotateCurrentTetrimino();
			break;
		case 'N':
			Timer->Enabled = true;
            SpawnNewTetrimino();
            break;
	}
	Repaint(); // Redraw the form with the new position or rotation
}
//---------------------------------------------------------------------------
void TMainForm::MoveTetriminoLeft()
{
	if (CanMove(tetrimino, posX - 1, posY))
		posX--;
}
//---------------------------------------------------------------------------

void TMainForm::MoveTetriminoRight()
{
	if (CanMove(tetrimino, posX + 1, posY))
		posX++;
}
//---------------------------------------------------------------------------

bool TMainForm::MoveTetriminoDown()
{
	if (CanMove(tetrimino, posX, posY + 1))
	{
		posY++;
		return true;
	}
	else
	{
		LockTetrimino();
		return false;
	}
}
//---------------------------------------------------------------------------


