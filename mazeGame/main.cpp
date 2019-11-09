#include <iostream>
#include <random>

struct GetRandomNumber
{
    static int returnRandomNumber(int min, int max)
    {
        static std::random_device randomDevice;
        static std::mt19937 engine{randomDevice()};
        std::uniform_int_distribution<int> distribution(min, max);

        return distribution(engine);
    }
};

class  Character
{
protected:
    int mX;
    int mY;

public:
    Character(int x, int y) : mX{x}, mY{y}
    {

    }

    int getX()
    {
        return mX;
    }

    int getY()
    {
        return mY;
    }
};

class Player : public Character
{
private:
    const char mSign = '@';

public:
     Player(int x, int y) : Character(x, y)
     {

     }

    void setNewPosition(int x, int y)
    {
        this->mX = x;
        this->mY = y;
    }

    char getCharacter()
    {
        return mSign;
    }
};

// треба доробити
class MakeMaze
{
private:
    char **maze;
public:
    MakeMaze(char **canvas)
        : maze{canvas}
    {

    }

    // 1 - up, 2 - down, 3 - left, 4 - right
};

class Key : public Character
{
private:
    const char mSign = 'k';

public:
    Key(int x, int y) : Character(x, y)
    {

    }

    char getCharacter()
    {
        return mSign;
    }
};

class PlayGround
{
private:
    char **mPlayGround;
    const int mWidth;
    const int mLength;

    int mPlayerMoveX;
    int mPlayerMoveY;

    Player mPlayer;
    Key mKey;

public:
    // length - довжина =>
    // width - ширина V
    PlayGround(int a, int b, int startPositionX = 1, int startPositionY = 1):
        mWidth{a}, mLength{b},
        mPlayer(startPositionX, startPositionY),
        mKey(GetRandomNumber::returnRandomNumber(2, mWidth - 2),
             GetRandomNumber::returnRandomNumber(2, mLength - 2))
    {
        char **m = new char *[mWidth];
        m[0] = new char[mWidth * mLength];

            for (int j(1); j != mWidth; ++j)
            {
                m[j] = m [j - 1] + mLength;
            }

            // i = y; j = x
            for (int i(0); i != mWidth; i++)
            {

                for (int j(0); j != mLength; j++)
                {
                    if (i == 0 || i == mWidth - 1)
                    {
                     m[i][j] = '#';

                    }
                    else if (j == 0 || j == mLength - 1)
                    {
                        m[i][j] = '#';
                    }
                    else
                    {
                        m[i][j] = ' ';
                    }
                }
            }

        MakeMaze Maze(m);

        //m = Maze.create(mPlayer.getX(), mPlayer.getY(), 3);
        m[mKey.getX()][mKey.getY()] = mKey.getCharacter();
        m[mPlayer.getY()][mPlayer.getX()] = mPlayer.getCharacter();

        mPlayGround = m;
        m = nullptr;
    }

    ~PlayGround()
    {
            delete [] mPlayGround[0];
            delete [] mPlayGround;
    }


    char **getPlayGround()
    {
        return mPlayGround;
    }

    void move(const char direction)
    {
        mPlayerMoveX = mPlayer.getX();
        mPlayerMoveY = mPlayer.getY();

        switch (direction)
        {
        case 'd':
             mPlayerMoveY++;
        break;

        case 'u':
                mPlayerMoveY--;
            break;

        case 'l':
                mPlayerMoveX--;
            break;

        case 'r':
                mPlayerMoveX++;
            break;

        default:
            std::cout << "I dont understand you\n";
        }
    }

    bool play(const char direction)
    {

        move(direction);

        if (mPlayGround[mPlayerMoveY][mPlayerMoveX] == mKey.getCharacter())
        {
            mPlayGround[mPlayer.getY()][mPlayer.getX()] = ' ';
            mPlayer.setNewPosition(mPlayerMoveX, mPlayerMoveY);
            return 1;
        }

        if ( mPlayGround[mPlayerMoveY][mPlayerMoveX] != '#')
        {
            mPlayGround[mPlayer.getY()][mPlayer.getX()] = ' ';
            mPlayer.setNewPosition(mPlayerMoveX, mPlayerMoveY);
            return 0;
        }

        return 0;
    }

    void vizualize ()
    {
        mPlayGround[mPlayer.getY()][mPlayer.getX()] = mPlayer.getCharacter();
        for (int i{0}; i != mWidth; i++)
        {
            for (int j{0}; j != mLength; j++)
            {
                std::cout << mPlayGround[i][j];
            }

            std::cout << '\n';
        }

    }
};

class UserInterface
{
public:

    static void userInterface()
    {
        int i(0);
        int j(0);

        bool endOfGame{0};

        while (true)
        {
        std::cout << "Input number of rows: ";

        while (!(std::cin >> i))
        {
            std::cout << "Error: input a number!\n";
            std::cin.clear();
            std::cin.ignore(3000, '\n');
            std::cout << "Input number of rows:\n";
        }

        std::cout << "Input number of columns: ";
        while (!(std::cin >> j))
        {
            std::cout << "Error: input a number!\n";
            std::cin.clear();
            std::cin.ignore(3000, '\n');
            std::cout << "Input number of columns:\n";
        }

        PlayGround A(i, j);

        A.vizualize();

        while (true)
        {
            std::cout << "Enter direction to move : (u - up, d - down, l - left, r - right)\n";
            std::cout << "Your input: ";

            char direction;
            std::cin >> direction;

            endOfGame = A.play(direction);
            A.vizualize();

            if (endOfGame)
            {
                std::cout << "Congratulations!\n";
                std::cout << "You win!\n";

                break;
            }
        }

        std::cout << "Play again? [Y/n]\n";
        char response = 'n';
        std::cout << "Your response: ";
        std::cin >> response;

        if (response == 'N' || response == 'n')
        {
            break;
        }
      }

    }
};

int main()
{
    UserInterface::userInterface();
    return 0;
}
