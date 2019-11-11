#include <iostream>
#include <random>

#define EXIT '#'
#define WALL 'X'
#define KEY 'k'
#define EMPTYSIGN ' '

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
    const char mSign{'@'};

    int  mPoints;
    bool mKeyIsFound;

public:
    Player(int x, int y, bool keyIsFound, int startPoints) : Character(x, y), mKeyIsFound{keyIsFound}, mPoints{startPoints}
     {

     }

    void changePoints(int change)
    {
        this->mPoints += change;

        if ( change <= 0)
        {
            if (mPoints <= 0)
            {
                mPoints = 0;
            }

        }

    }

    int getPoints()
    {
        return mPoints;
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

    void takeKey()
    {
        this->mKeyIsFound = 1;
    }

    bool hasKey()
    {
        return mKeyIsFound;
    }
};

class PrizeIteam
{
private:
    int mSign;

    const char mFirstSign {'1'};
    const char mSecondSign {'2'};
    const char mThirdSign {'3'};

    const int mFirstPrize {10};
    const int mSecondPrize{25};
    const int mThirdPrize{35};

public:
    PrizeIteam(int sign = 1): mSign{sign}
    {

    }

    char getSign()
    {

        switch (mSign)
        {
        case 1:
            return  mFirstSign;
        case 2:
            return  mSecondSign;
        case 3:
            return mThirdSign;
        }
        return EMPTYSIGN;
    }

    char changePrizeSign()
    {
        mSign++;

        switch (mSign)
        {
        case 1:
            return  mFirstSign;
        case 2:
            return  mSecondSign;
        case 3:
            return mThirdSign;
        }
        return EMPTYSIGN;
    }

    int deservedPoints()
    {
        switch (mSign)
        {
        case 1:
            return  mFirstPrize;
        case 2:
            return  mSecondPrize;
        case 3:
            return mThirdPrize;
        }

        return 0;
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
};

class PlayGround
{
private:
    char **mPlayGround;

    int mWidth;
    int mLength;

    int mPlayerMoveX;
    int mPlayerMoveY;

    int mLevel;

    Player mPlayer;

    PrizeIteam mPrize;

public:
    // length - довжина =>
    // width - ширина V
    PlayGround(int a, int b, int startPositionX = 1, int startPositionY = 1, bool keyIsFound = 0, int startLevel = 1):
        mWidth{a}, mLength{b}, mLevel{1},
        mPlayer(startPositionX, startPositionY, keyIsFound, a * b / 2)
    {

    }

    ~PlayGround()
    {
            delete [] mPlayGround[0];
            delete [] mPlayGround;
    }

    bool upgradeLevel()
    {
        if (mLength / mLevel > 2 && mWidth / mLevel > 0)
        {
            mLevel++;
            mPlayer.changePoints(-1000);
            mPlayer.changePoints(mWidth * mLength / 2);
            return 1;
        }
        return 0;
    }

    bool createPlayGround()
    {

        mWidth /= mLevel;
        mLength /= mLevel;

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
                    m[i][j] = WALL;

                }
                else if (j == 0 || j == mLength - 1)
                {
                    m[i][j] = WALL;
                }
                else
                {
                    m[i][j] = ' ';
                }
            }
        }

        //MakeMaze Maze(m);

        //m = Maze.create(mPlayer.getX(), mPlayer.getY(), 3);

        m[GetRandomNumber::returnRandomNumber(2, mWidth - 2)]
                [GetRandomNumber::returnRandomNumber(2, mWidth - 2)] = mPrize.getSign();
        m[GetRandomNumber::returnRandomNumber(2, mWidth - 2)]
                [GetRandomNumber::returnRandomNumber(2, mLength - 2)] = KEY;

        m[mWidth - 1][mLength/2] = EXIT;
        m[mPlayer.getY()][mPlayer.getX()] = mPlayer.getCharacter();

        mPlayGround = m;
        m = nullptr;
    }

    int getPlayerPoints()
    {
        return mPlayer.getPoints();
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

        int decreaseMovePoints = -1 * mLevel;

        mPlayer.changePoints(decreaseMovePoints);

        if (mPlayGround[mPlayerMoveY][mPlayerMoveX] == KEY)
        {
            mPlayGround[mPlayer.getY()][mPlayer.getX()] = EMPTYSIGN;
            mPlayer.setNewPosition(mPlayerMoveX, mPlayerMoveY);
            mPlayer.takeKey();
        }

        else if (mPlayGround[mPlayerMoveY][mPlayerMoveX] == EXIT && mPlayer.hasKey())
        {
            mPlayGround[mPlayer.getY()][mPlayer.getX()] = EMPTYSIGN;
            mPlayer.setNewPosition(mPlayerMoveX, mPlayerMoveY);
            return 1;
        }

        else if (mPlayGround[mPlayerMoveY][mPlayerMoveX] == mPrize.getSign())
        {
            mPlayer.changePoints(mPrize.deservedPoints());
            mPrize.changePrizeSign();

            mPlayGround[GetRandomNumber::returnRandomNumber(2, mWidth - 2)]
                    [GetRandomNumber::returnRandomNumber(2, mWidth - 2)]  = mPrize.getSign();

            mPlayGround[mPlayer.getY()][mPlayer.getX()] = EMPTYSIGN;


            mPlayer.setNewPosition(mPlayerMoveX, mPlayerMoveY);
        }

        if ( (mPlayGround[mPlayerMoveY][mPlayerMoveX] == WALL) ||
             ((!mPlayer.hasKey()) && mPlayGround[mPlayerMoveY][mPlayerMoveX] == EXIT))
        {
            return 0;
        }

        mPlayGround[mPlayer.getY()][mPlayer.getX()] = EMPTYSIGN;
        mPlayer.setNewPosition(mPlayerMoveX, mPlayerMoveY);
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
        A.createPlayGround();
        A.vizualize();

        while (true)
        {
            std::cout << "Your points: " << A.getPlayerPoints() << '\n';
            std::cout << "Enter direction to move : (u - up, d - down, l - left, r - right, q - quit)\n";
            std::cout << "Your input: ";

            char direction;
            std::cin >> direction;

            if (direction == 'q')
            {
                break;
            }
            else
            {
                endOfGame = A.play(direction);
            }

            A.vizualize();

            if (endOfGame)
            {
                std::cout << "Congratulations!\n";
                std::cout << "You win!\n";
                std::cout << "Your Points: " << A.getPlayerPoints() << '\n';
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
