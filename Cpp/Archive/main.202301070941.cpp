/*SudokuSolver - V1.0 - Jan05, 2023
*/
#include<iostream>
#include<string>
#include<sstream>
#include<cstdlib>

using namespace std;

const int EMPTY = 0;
enum Values { NO, YES, MAYBE };
enum Dimensions { ROW, COL, BLOCK };

struct Coords {
  int Row, Col, Count;
};

void ShowGrid( char Grid[][9][9] );
char ParseCommand( string s, int &Row, int &Col, int &Value );
bool CheckGrid( char Grid[][9][9], bool Verbose=false );
bool SetValue( char Grid[][9][9], int Row, int Col, int Value,
               bool Permanent );
bool IsValidRow( char Grid[][9][9], int Index, int &Value );
bool IsValidCol( char Grid[][9][9], int Index, int &Value );
bool IsValidBlock( char Grid[][9][9], int Index, int &Value );
int  FindBest( char Grid[][9][9], int &MaxDigitCount, int &Index );
void Sort( Coords Stack[], int StackSize );

//===========================================================================
int main( int argc, char** argv ) {
  //Initialize grid.
  char Grid[9][9][9];
  for( int i = 0; i < 9; i++ )
    for( int j = 0; j < 9; j++ ) {
      Grid[i][j][0] = EMPTY;        //Index 0 is the actual value in the cell
      for( int k = 1; k <= 9; k++ ) //Idxs 1-9 are candidate values
        Grid[i][j][k] = MAYBE;
    }

  //Get initial grid setup from user.
  string s;
  char   command = '?';
  int    row, col;
  int    val;
  ShowGrid( Grid );
  
  do {
    if( command == '?' )
      cout << "\nPlease enter the initial setup of the puzzle. Type:\n"
           << "  * 'a' - followed by a digit, then a pair of coordinates "
           << "(row, column) to add a digit.\n"
           << "          E.g. 'a 7 5 4' puts a '4' in the 7th row, 5th "
           << "column.\n"
           << "  * 'd' - followed by a pair of coordinates to delete a digit"
           << "\n"
           << "          E.g. 'd 3 8' deletes the digit (if there is one) in "
           << "the 3rd row, 8th column.\n"
           << "  * 'p' - to print the grid.\n"
           << "  * 's' - to solve the puzzle.\n"
           << "  * 'q' - to quit.\n"
           << "  * '?' - to display these instructions again." << endl;
    else if( command == 'p' )
      ShowGrid( Grid );
    else if( command == 'a' ) {
      SetValue( Grid, row, col, val, true );
    }
    else if( command == 'd' ) {
      if( row >= 1 && row <= 9 && col >= 1 && col <= 9 )
          Grid[row-1][col-1][0] = EMPTY;
      else
        cout << "** Coordinates are out-of-bounds! Please try again. **"
             << endl;
    }

    cout << "Command: ";
    getline( cin, s );
    command = ParseCommand( s, row, col, val );

    if( command != 'a' && command != 'd' && command != 'p' &&
        command != 's' && command != '?' && command != 'q' ) {
      cout << "** Unrecognized command! Please try again. ** " << endl;
    }
    else if( command == 's' ) {
      //Check current grid setup is valid.
      if( ! CheckGrid(Grid, true) ) {
        cout << "** The current grid is invalid! Please check it and try "
             << "again. **" << endl;
        command = ' ';
      }
    }
  } while( command != 's' && command != 'q' );

  if( command == 'q' ) {
    cout << "Bye!" << endl;
    return 0;    
  }

  //Solve the puzzle.
  //Stack coordinates in order of solvability (hardest to easiest).
  Coords Stack[81];
  int  StackSize = 0;
  for( int i = 0; i < 9; i++ ) {
    for( int j = 0; j < 9; j++ ) {
      int count = 0;
      bool already_solved = false;
      for( int k = 1; k <= 9; k++ ) {
        if( Grid[i][j][k] == MAYBE )
          count++;
        else if( Grid[i][j][k] == YES )
          already_solved = true;
      }

      if( count > 0 || ! already_solved ) {
        Stack[StackSize].Row = i;
        Stack[StackSize].Col = j;        
        Stack[StackSize].Count = count;
        StackSize++;
      }
    }
  }

  Sort( Stack, StackSize );

  return 0;
} //main

//---------------------------------------------------------------------------
char ParseCommand( string s, int &Row, int &Col, int &Value ) {
  char c;
  istringstream is( s );
  is >> c;
  c = tolower( c );

  if( c == '?' || c == 's' || c == 'p' || c == 'q' )
    return c;

  if( c == 'a' )
    is >> Row >> Col >> Value;
  else if( c == 'd' )
    is >> Row >> Col;

  return c;
} //ParseCommand

//---------------------------------------------------------------------------
void ShowGrid( char Grid[][9][9] ) {
  for( int i = 0; i < 9; i++ ) {
    if( i == 0 ) {  //Write column headers
      cout << "  ";
      for( int j = 1; j <= 9; j++ ) {
        cout << j;
        cout << ' ';
        if( j % 3 == 0 ) cout << ' ';
      }
      cout << endl;
    }

    if( i % 3 == 0 ) {         //Write row dividers
      cout << "  ";
      for( int j = 0; j < (9*2)+2; j++ )
        cout << "-";
      cout << endl;
    }

    cout << (i+1);
    for( int j = 0; j < 9; j++ ) {
      if( j % 3 == 0 ) cout << '|';
      if( Grid[i][j][0] == EMPTY )
        cout << ". ";
      else
        cout << (int) Grid[i][j][0] << ' ';
    }
    cout << endl;
  }
} //ShowGrid

//---------------------------------------------------------------------------
bool CheckGrid( char Grid[][9][9], bool Verbose ) {
  //Check for invalid digit values.
  for( int i = 0; i < 9; i++ ) {
    for( int j = 0; j < 9; j++ ) {
      int val = Grid[i][j][0];
      if( val == EMPTY ) {}
      else if( val < 0 || val > 9 ) {
        if( Verbose )
          cout << "CheckGrid: ** Invalid value at (" << (i+1) << ", " << (j+1)
               << "): " << val << "! **" << endl;
        return false;
      }
    }
  }

  int val;

  //Check rows.
  for( int i = 0; i < 9; i++ ) {
    if( ! IsValidRow(Grid, i, val) ) {
      if( Verbose )
        cout << "CheckGrid: ** " << val << " appears more than once in "
             << "row " << (i+1) << "! **" << endl;
        return false;
    }
  }

  //Check columns.
  for( int j = 0; j < 9; j++ ) {
    if( ! IsValidCol(Grid, j, val) ) {
      if( Verbose )
        cout << "CheckGrid: ** " << val << " appears more than once in "
             << "column " << (j+1) << "! **" << endl;
      return false;
    }
  }

  //Check 3x3 blocks.
  for( int m = 0; m < 9; m++ ) {
    if( ! IsValidBlock(Grid, m, val) ) {
      if( Verbose )
        cout << "CheckGrid: ** " << val << " appears more than once in "
             << "a block" << "! **" << endl;
      return false;
    }
  }

  return true;
} //CheckGrid

//---------------------------------------------------------------------------
bool SetValue( char Grid[][9][9], int Row, int Col, int Value,
               bool Permanent=false ) {
  if( Row >= 1 && Row <= 9 && Col >= 1 && Col <= 9 ) {
    if( Value >= 1 && Value <= 9 ) {
      Grid[Row-1][Col-1][0] = (char) Value;

      for( int k = 1; k <= 9; k++ ) {
        if( k == Value )
          Grid[Row-1][Col-1][k] = YES;
        else
          Grid[Row-1][Col-1][k] = NO;
      }

      if( Permanent ) {
        //Remove value in other col cells.
        for( int i = 0; i < 9; i++ )
          if( i != Row-1 )
            Grid[i][Col-1][Value-1] = NO;

        //Remove value in other row cells.
        for( int j = 0; j < 9; j++ )
          if( j != Col-1 )
            Grid[Row-1][j][Value-1] = NO;

        //Remove value in other block cells.
        int m = (Row-1) % 3;
        int n = (Col-1) % 3;
        for( int i = m; i < m+3; i++ ) {
          for( int j = n; j < n+3; j++ ) {
            if( i != Row-1 && j != Col-1 )
              Grid[i][j][Value-1] = NO;
          }
        }
      }

      return true;
    }
    else
      cout << "SetValue: ** Invalid digit value! Please try again. **"
           << endl;
  }
  else
    cout << "SetValue: ** Coordinates are out-of-bounds! Please try again. **"
         << endl;

  return false;
} //SetValue

//---------------------------------------------------------------------------
bool IsValidRow( char Grid[][9][9], int Index, int &Value ) {
  bool seen[9];
  for( int k = 0; k < 9; k++ )
    seen[k] = false;

  for( int j = 0; j < 9; j++ ) {
    Value = Grid[Index][j][0];
    if( Value != EMPTY ) {
      if( ! seen[Value-1] )
        seen[Value-1] = true;
      else
        return false;
    }
  }

  return true;
} //IsValidRow

//---------------------------------------------------------------------------
bool IsValidCol( char Grid[][9][9], int Index, int &Value ) {
  bool seen[9];
  for( int k = 0; k < 9; k++ )
    seen[k] = false;

  for( int i = 0; i < 9; i++ ) {
    Value = Grid[i][Index][0];
    if( Value != EMPTY ) {
      if( ! seen[Value-1] )
        seen[Value-1] = true;
      else
        return false;
    }
  }

  return true;
} //IsValidRow

//---------------------------------------------------------------------------
bool IsValidBlock( char Grid[][9][9], int Index, int &Value ) {
  bool seen[9];
  for( int k = 0; k < 9; k++ )
    seen[k] = false;

  for( int i = 0; i < 3; i++ ) {
    for( int j = 0; j < 3; j++ ) {
      Value = Grid[Index/3*3+i][Index%3*3+j][0];
      if( Value != EMPTY ) {
        if( ! seen[Value-1] )
          seen[Value-1] = true;
        else
          return false;
      }
    }
  }

  return true;
} //IsValidBlock

//---------------------------------------------------------------------------
void Sort( Coords Stack[], int StackSize ) {
  //AAA
} //Sort

//---------------------------------------------------------------------------
int  FindBest( char Grid[][9][9], int &MaxDigitCount, int &Index ) {
  int MaxConfirmedDigitsRow = 0, BestRow;
  for( int i = 0; i < 9; i++ ) {
    int count = 0;
    for( int j = 0; j < 9; j++ ) {
      if( Grid[i][j][0] != EMPTY )
        count++;
    }
    if( count > MaxConfirmedDigitsRow ) {
      MaxConfirmedDigitsRow = count;
      BestRow = i;
    }
  }

  int MaxConfirmedDigitsCol = 0, BestCol;
  for( int j = 0; j < 9; j++ ) {
    int count = 0;
    for( int i = 0; i < 9; i++ ) {
      if( Grid[i][j][0] != EMPTY )
        count++;
    }
    if( count > MaxConfirmedDigitsCol ) {
      MaxConfirmedDigitsCol = count;
      BestCol = j;
    }
  }

  int MaxConfirmedDigitsBlock = 0, BestBlock;
  for( int k = 0; k < 9; k++ ) {
    int count = 0;
    for( int l = 0; l < 9; l++ ) {
      if( Grid[k/3*3+l/3][k%3*3+l%3][0] != EMPTY )
        count++;
    }
    if( count > MaxConfirmedDigitsBlock ) {
      MaxConfirmedDigitsBlock = count;
      BestBlock = k;
    }
  }

  Index = BestRow;
  MaxDigitCount = MaxConfirmedDigitsRow;
  int Type = ROW;
  if( MaxConfirmedDigitsCol > MaxDigitCount ) {
    Index = BestCol;
    MaxDigitCount = MaxConfirmedDigitsCol;
    Type = COL;
  }
  if( MaxConfirmedDigitsBlock > MaxDigitCount ) {
    Index = BestBlock;
    MaxDigitCount = MaxConfirmedDigitsBlock;
    Type = BLOCK;
  }

  return Type;
} //FindBest

