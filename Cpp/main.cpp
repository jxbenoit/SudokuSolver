/*SudokuSolver - V1.0 - Jan05, 2023
*/
#include<iostream>
#include<string>
#include<sstream>
#include<cstdlib>

using namespace std;

const int EMPTY = 0;
enum Dimensions { ROW, COL, BLOCK };

struct Coords {
  int I, J, Count;
};

void ShowGrid( char** Grid );
char ParseCommand( string s, int &Row, int &Col, int &Value );
bool SetValue( char** Grid, bool*** Digits, int Row, int Col,
               int Value, bool Permanent );
bool CheckGrid( char** Grid, bool Verbose=false );
bool IsValidRow( char** Grid, int Index, int &Value );
bool IsValidCol( char** Grid, int Index, int &Value );
bool IsValidBlock( char** Grid, int Index, int &Value );
bool Solve( char** Grid, bool*** Digits );
int  CountEmptyCells( char** Grid );
char** MakeNewGrid();
void  CopyGrid( char** Grid1, char** Grid2 );
void  CopyDigitsArray( bool*** Digits1, bool*** Digits2 );
bool*** MakeNewDigitsArray();

//===========================================================================
int main( int argc, char** argv ) {
  //Initialize grid & digits array.
  bool*** Digits = MakeNewDigitsArray();
  char** Grid = MakeNewGrid();

//AAA
//SetValue( Grid, Digits, 1, 5, 4, true );
//SetValue( Grid, Digits, 1, 6, 3, true );
//SetValue( Grid, Digits, 2, 3, 2, true );
//SetValue( Grid, Digits, 2, 6, 6, true );
//SetValue( Grid, Digits, 2, 9, 8, true );
//SetValue( Grid, Digits, 3, 4, 2, true );
//SetValue( Grid, Digits, 3, 7, 1, true );
//SetValue( Grid, Digits, 3, 8, 5, true );
//SetValue( Grid, Digits, 4, 2, 3, true );
//SetValue( Grid, Digits, 4, 3, 9, true );
//SetValue( Grid, Digits, 4, 5, 2, true );
//SetValue( Grid, Digits, 4, 7, 5, true );
//SetValue( Grid, Digits, 5, 3, 8, true );
//SetValue( Grid, Digits, 5, 4, 6, true );
//SetValue( Grid, Digits, 5, 5, 3, true );
//SetValue( Grid, Digits, 5, 6, 1, true );
//SetValue( Grid, Digits, 5, 7, 9, true );
//SetValue( Grid, Digits, 6, 3, 7, true );
//SetValue( Grid, Digits, 6, 5, 8, true );
//SetValue( Grid, Digits, 6, 7, 3, true );
//SetValue( Grid, Digits, 6, 8, 6, true );
//SetValue( Grid, Digits, 7, 2, 8, true );
//SetValue( Grid, Digits, 7, 3, 6, true );
//SetValue( Grid, Digits, 7, 6, 2, true );
//SetValue( Grid, Digits, 8, 1, 4, true );
//SetValue( Grid, Digits, 8, 4, 1, true );
//SetValue( Grid, Digits, 8, 7, 2, true );
//SetValue( Grid, Digits, 9, 4, 3, true );
//SetValue( Grid, Digits, 9, 5, 6, true );
SetValue( Grid, Digits, 1, 3, 7, true );
SetValue( Grid, Digits, 1, 7, 4, true );
SetValue( Grid, Digits, 1, 8, 6, true );
SetValue( Grid, Digits, 2, 2, 8, true );
SetValue( Grid, Digits, 2, 5, 9, true );
SetValue( Grid, Digits, 2, 6, 4, true );
SetValue( Grid, Digits, 2, 9, 2, true );
SetValue( Grid, Digits, 3, 5, 2, true );
SetValue( Grid, Digits, 3, 6, 1, true );
SetValue( Grid, Digits, 4, 1, 7, true );
SetValue( Grid, Digits, 4, 2, 6, true );
SetValue( Grid, Digits, 4, 7, 5, true );
SetValue( Grid, Digits, 5, 5, 4, true );
SetValue( Grid, Digits, 6, 3, 5, true );
SetValue( Grid, Digits, 6, 8, 2, true );
SetValue( Grid, Digits, 6, 9, 3, true );
SetValue( Grid, Digits, 7, 4, 4, true );
SetValue( Grid, Digits, 7, 5, 8, true );
SetValue( Grid, Digits, 8, 1, 1, true );
SetValue( Grid, Digits, 8, 4, 9, true );
SetValue( Grid, Digits, 8, 5, 7, true );
SetValue( Grid, Digits, 8, 8, 4, true );
SetValue( Grid, Digits, 9, 2, 9, true );
SetValue( Grid, Digits, 9, 3, 2, true );
SetValue( Grid, Digits, 9, 7, 3, true );
//AAA

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
      SetValue( Grid, Digits, row, col, val, true );
    }
    else if( command == 'd' ) {
      if( row >= 1 && row <= 9 && col >= 1 && col <= 9 ) {
        Grid[row-1][col-1] = EMPTY;
        for( int k = 0; k < 9; k++ )
          Digits[row-1][col-1][k] = true;
      }
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
  bool Solved = Solve( Grid, Digits );

  if( Solved ) {
    cout << "Solved!" << endl;
    ShowGrid( Grid );
  }
  else
    cout << "Sorry! The puzzle is unsolvable." << endl;

  //Clean up.
  for( int i = 0; i < 9; i++ )
    delete [] Grid[i];
  delete [] Grid;

  for( int i = 0; i < 9; i++ ) {
    for( int j = 0; j < 9; j++ )
      delete [] Digits[i][j];
    delete [] Digits[i];
  }
  delete [] Digits;

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
void ShowGrid( char** Grid ) {
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
      if( Grid[i][j] == EMPTY )
        cout << ". ";
      else
        cout << (int) Grid[i][j] << ' ';
    }
    cout << endl;
  }
} //ShowGrid

//---------------------------------------------------------------------------
bool CheckGrid( char** Grid, bool Verbose ) {
  //Check for invalid digit values.
  for( int i = 0; i < 9; i++ ) {
    for( int j = 0; j < 9; j++ ) {
      int val = Grid[i][j];
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
bool SetValue( char** Grid, bool*** Digits, int Row, int Col,
               int Value, bool Permanent=false ) {
  if( Row >= 1 && Row <= 9 && Col >= 1 && Col <= 9 ) {
    if( Value >= 1 && Value <= 9 ) {
      Grid[Row-1][Col-1] = (char) Value;

      if( Permanent ) {
        //Remove any other possible value for this cell.
        for( int k = 0; k < 9; k++ ) {
          if( k == Value-1 )
            Digits[Row-1][Col-1][k] = true;
          else
            Digits[Row-1][Col-1][k] = false;
        }

        //Remove value in other col cells.
        for( int i = 0; i < 9; i++ )
          if( i != Row-1 )
            Digits[i][Col-1][Value-1] = false;

        //Remove value in other row cells.
        for( int j = 0; j < 9; j++ )
          if( j != Col-1 )
            Digits[Row-1][j][Value-1] = false;

        //Remove value in other block cells.
        int m = (Row-1) / 3 * 3;
        int n = (Col-1) / 3 * 3;
        for( int i = m; i < m+3; i++ ) {
          for( int j = n; j < n+3; j++ ) {
            if( i != Row-1 || j != Col-1 ) {
              Digits[i][j][Value-1] = false;
            }
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
bool IsValidRow( char** Grid, int Index, int &Value ) {
  bool seen[9];
  for( int k = 0; k < 9; k++ )
    seen[k] = false;

  for( int j = 0; j < 9; j++ ) {
    Value = Grid[Index][j];
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
bool IsValidCol( char** Grid, int Index, int &Value ) {
  bool seen[9];
  for( int k = 0; k < 9; k++ )
    seen[k] = false;

  for( int i = 0; i < 9; i++ ) {
    Value = Grid[i][Index];
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
bool IsValidBlock( char** Grid, int Index, int &Value ) {
  bool seen[9];
  for( int k = 0; k < 9; k++ )
    seen[k] = false;

  for( int i = 0; i < 3; i++ ) {
    for( int j = 0; j < 3; j++ ) {
      Value = Grid[Index/3*3+i][Index%3*3+j];
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
int  CountEmptyCells( char** Grid ) {
  int count = 0;
  for( int i = 0; i < 9; i++ )
    for( int j = 0; j < 9; j++ )
      if( Grid[i][j] == EMPTY )
        count++;

  return count;
} //CountEmptyCells

//---------------------------------------------------------------------------
char** MakeNewGrid() {
  char** Grid = new char* [9];
  for( int i = 0; i < 9; i++ ) {
    Grid[i] = new char [9];
    for( int j = 0; j < 9; j++ )
      Grid[i][j] = EMPTY;           //Actual digit in the cell
  }
  return Grid;
} //MakeNewGrid

//---------------------------------------------------------------------------
bool*** MakeNewDigitsArray() {
  bool*** Digits = new bool** [9];
  for( int i = 0; i < 9; i++ ) {
    Digits[i] = new bool* [9];
    for( int j = 0; j < 9; j++ ) {
      Digits[i][j] = new bool [9];
      for( int k = 0; k < 9; k++ )
        Digits[i][j][k] = true;    //Candidate digits for each cell
    }   
  }
  return Digits;
} //MakeNewDigitsArray

//---------------------------------------------------------------------------
void  CopyGrid( char** Grid1, char** Grid2 ) {
  for( int i = 0; i < 9; i++ )
    for( int j = 0; j < 9; j++ )
      Grid2[i][j] = Grid1[i][j];
} //CopyGrid

//---------------------------------------------------------------------------
void  CopyDigitsArray( bool*** Digits1, bool*** Digits2 ) {
  for( int i = 0; i < 9; i++ )
    for( int j = 0; j < 9; j++ )
      for( int k = 0; k < 9; k++ )
        Digits2[i][j][k] = Digits1[i][j][k];
} //CopyDigitsArray

//---------------------------------------------------------------------------
bool Solve( char** Grid, bool*** Digits ) {
  //Find one of the easiest cells to solve.
  Coords Cell;
  int LowestDigitsCount = 10;
  for( int i = 0; i < 9; i++ ) {
    for( int j = 0; j < 9; j++ ) {
      if( Grid[i][j] == EMPTY ) {
        int count = 0, last_value;
        for( int k = 0; k < 9; k++ )
          if( Digits[i][j][k] == true ) {
            count++;
            last_value = k + 1;
          }

        //If no eligible digits for this cell, the grid is invalid.
        if( count == 0 ) {
          return false;
        }
        else if( count == 1 ) {  //Only one possible value for the cell
          //Test setting this value.
          SetValue( Grid, Digits, i+1, j+1, last_value );

          //Check if this newly-set value is valid in the grid.
          int  bad_val;
          if( ! IsValidRow(Grid, i, bad_val) ||
              ! IsValidCol(Grid, j, bad_val) ||
              ! IsValidBlock(Grid, (i/3*3+j/3), bad_val) ) {
            return false;
          }

          //It's good -- permanently set the value.
          SetValue( Grid, Digits, i+1, j+1, last_value, true );
        }
        else if( count < LowestDigitsCount ) {
          LowestDigitsCount = count;
          Cell.I = i;
          Cell.J = j;
        }
      }
    }
  }

  if( LowestDigitsCount == 10 ) {
    //No cells left with digits to determine. All cells are set.
    return true;
  }

  //Go through possible values for the selected cell.
  char** TempGrid = MakeNewGrid();
  bool*** TempDigits = MakeNewDigitsArray();
  for( int k = 0; k < 9; k++ ) {
    if( Digits[Cell.I][Cell.J][k] ) {
      CopyGrid( Grid, TempGrid );
      CopyDigitsArray( Digits, TempDigits );

      //Test setting this value.
      SetValue( TempGrid, TempDigits, Cell.I+1, Cell.J+1, k+1, true );

      //Check if this newly-set value is valid in the grid.
      int  bad_val;
      if( ! IsValidRow(Grid, Cell.I, bad_val) ||
          ! IsValidCol(Grid, Cell.J, bad_val) ||
          ! IsValidBlock(Grid, (Cell.I/3*3+Cell.J/3), bad_val) ) {
        //Delete temporary grid.
        for( int i = 0; i < 9; i++ )
          delete [] TempGrid[i];
        delete [] TempGrid;

        for( int i = 0; i < 9; i++ ) {
          for( int j = 0; j < 9; j++ )
            delete [] TempDigits[i][j];
          delete [] TempDigits[i];
        }
        delete [] TempDigits;

        return false;
      }

ShowGrid(TempGrid); //AAA
char a;             //AAA
cin >> a;           //AAA

      if( Solve(TempGrid, TempDigits) ) {
        CopyGrid( TempGrid, Grid );
        CopyDigitsArray( TempDigits, Digits );

        //Delete temporary grid.
        for( int i = 0; i < 9; i++ )
          delete [] TempGrid[i];
        delete [] TempGrid;

        for( int i = 0; i < 9; i++ ) {
          for( int j = 0; j < 9; j++ )
            delete [] TempDigits[i][j];
          delete [] TempDigits[i];
        }
        delete [] TempDigits;

        return true;
      }
    }
  }  //End loop through k

  //Delete temporary grid.
  for( int i = 0; i < 9; i++ )
    delete [] TempGrid[i];
  delete [] TempGrid;

  for( int i = 0; i < 9; i++ ) {
    for( int j = 0; j < 9; j++ )
      delete [] TempDigits[i][j];
    delete [] TempDigits[i];
  }
  delete [] TempDigits;

  return false;
} //Solve