//---------------------------------------------------------------------------

#ifndef MainUnitH
#define MainUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <array>

// Define the size of each Tetrimino grid
const int TETRIMINO_SIZE = 4;
// Define block size for rendering
const int BLOCK_SIZE = 20;
// Enum for each Tetrimino type
enum ShapeType { I, O, T, S, Z, J, L };

const TColor I_TETRIMINO_COLOR = TColor(RGB(0, 255, 255));  // Cyan
const TColor O_TETRIMINO_COLOR = TColor(RGB(255, 255, 0));  // Yellow
const TColor T_TETRIMINO_COLOR = TColor(RGB(128, 0, 128));  // Purple
const TColor S_TETRIMINO_COLOR = TColor(RGB(0, 255, 0));    // Green
const TColor Z_TETRIMINO_COLOR = TColor(RGB(255, 0, 0));    // Red
const TColor J_TETRIMINO_COLOR = TColor(RGB(0, 0, 255));    // Blue
const TColor L_TETRIMINO_COLOR = TColor(RGB(255, 165, 0));  // Orange


// Structure for a Tetrimino
struct Tetrimino {
	ShapeType type;
	TColor color;
	std::array<std::array<int, TETRIMINO_SIZE>, TETRIMINO_SIZE> grid;

	// Default constructor
	Tetrimino() : type(I), grid{}, color{} {}

	Tetrimino(ShapeType t, const std::array<std::array<int, TETRIMINO_SIZE>, TETRIMINO_SIZE>& g, TColor c)
		: type(t), grid(g), color(c) {}
};

Tetrimino shapes[] = {
    Tetrimino(ShapeType::I, {{
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    }}, I_TETRIMINO_COLOR),

    Tetrimino(ShapeType::O, {{
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    }}, O_TETRIMINO_COLOR),

    Tetrimino(ShapeType::T, {{
        {0, 0, 0, 0},
        {0, 1, 1, 1},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    }}, T_TETRIMINO_COLOR),

    Tetrimino(ShapeType::S, {{
        {0, 0, 0, 0},
        {0, 0, 1, 1},
        {0, 1, 1, 0},
        {0, 0, 0, 0}
    }}, S_TETRIMINO_COLOR),

    Tetrimino(ShapeType::Z, {{
        {0, 0, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 0}
    }}, Z_TETRIMINO_COLOR),

    Tetrimino(ShapeType::J, {{
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0}
    }}, J_TETRIMINO_COLOR),

    Tetrimino(ShapeType::L, {{
        {0, 0, 0, 0},
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0}
    }}, L_TETRIMINO_COLOR)
};


//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TTimer *Timer;
	TPaintBox *PaintBoxNextPiece;
	TPaintBox *LevelPaintBox;
	TPaintBox *ScorePaintBox;
	TPaintBox *GameBoardPaintBox;
	TPaintBox *InfoPaintBox;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// User declarations
	// Constants for grid dimensions
	static constexpr int GRID_WIDTH = 10;  // Width of the grid in blocks
	static constexpr int GRID_HEIGHT = 20; // Height of the grid in blocks

    // Define the grid array
	int grid[GRID_HEIGHT][GRID_WIDTH] = {0};

	Tetrimino tetrimino;   // Active Tetrimino piece
	Tetrimino nextTetrimino; // Stores the next Tetrimino piece
	int largerBlockSize = BLOCK_SIZE * 2;
    int posX = 0;          // X position of the active Tetrimino
	int posY = 0;          // Y position of the active Tetrimino
	int gridX;
	int gridY;
	// Position of the active Tetrimino
	int tetriminoX = 4;
	int tetriminoY = 0;
	int pieceCount = 0;
    int lineCount = 0;

	void InitializeGrid();
	void SetCell(int x, int y, int value);
	int GetCell(int x, int y);
	void DrawBlock(TCanvas* Canvas, int x, int y, TColor color);

	// Game Engine
	bool CanMove(const Tetrimino& tetrimino, int newX, int newY);
	void MoveTetriminoLeft();
	void MoveTetriminoRight();
	bool MoveTetriminoDown();
	Tetrimino RotateTetrimino(const Tetrimino& tetrimino);
	// Checks if the Tetrimino can move to a specific position

	void LockTetrimino();
	void ClearLines();
	void SpawnNewTetrimino();

	void RotateCurrentTetrimino();
	void RenderNextTetrimino();
	void RenderLevel();
	void RenderInfoBox();
	void RenderScore();
	void RenderGrid();
	void RenderTetrimino();

    TColor ColorGrid[GRID_HEIGHT][GRID_WIDTH]; // Grid to store the colors of the blocks

	TBitmap *gameBoardBuffer;
    TBitmap *levelBuffer;
	TBitmap *nextPieceBuffer; // Off-screen buffer for the next piece preview
	TBitmap *scoreBuffer;     // Off-screen buffer for the score display
    TBitmap *infoBuffer;

    void __fastcall WMEraseBkgnd(TMessage &Message);
	BEGIN_MESSAGE_MAP
    	VCL_MESSAGE_HANDLER(WM_ERASEBKGND, TMessage, WMEraseBkgnd)
	END_MESSAGE_MAP(TForm)
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	int offsetX;
	int offsetY;

	int level = 1; // Stores the current level
	int score = 0; // Stores the current score
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
