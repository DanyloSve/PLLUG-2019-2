#include <iostream>
#include <random>

#define EXIT '#'
#define WALL 'X'
#define KEY 'k'
#define BLACK_HOLE 'O' // moves player to random place
#define ENEMY '$' // if you touch it, you'll fail
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


class Player
{
private:
    const char mSign{'@'};

    int mX;
    int mY;
    int  mPoints;
    bool mKeyIsFound;

public:
    Player(bool keyIsFound, int startPoints) : mKeyIsFound{keyIsFound}, mPoints{startPoints}
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

    int getX()
    {
        return mX;
    }

    int getY()
    {
        return mY;
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

// тут треба зробити лабіринт
class MakeMaze
{
private:

public:
};

class PlayGround
{
private:
    char **mPlayGround;

    int mWidth;
    int mLength;

    int mPlayerMoveX;
    int mPlayerMoveY;

    double mLevel;

    Player mPlayer;

    PrizeIteam mPrize;

public:
    // length - довжина =>
    // width - ширина V
    PlayGround(int beginningWidth, int beginningLength, bool keyIsFound = 0, double startLevel = 1):
        mWidth{beginningWidth}, mLength{beginningLength}, mLevel{startLevel},
        mPlayer(keyIsFound, beginningWidth * beginningLength / 2)
    {

    }

    ~PlayGround()
    {
            delete [] mPlayGround[0];
            delete [] mPlayGround;
    }

    void startFromBeginning(int width, int length)
    {
        this->mWidth = width;
        this->mLength = length;

        mLevel = 1;
    }

    void upgradeLevel()
    {
        mPlayer.changePoints(mWidth + mPlayer.getPoints());
        mLevel += 0.5;
    }

    void setCharacters()
    {
        int startPosition {1};
        mPlayer.setNewPosition(startPosition,startPosition);
        mPlayGround[GetRandomNumber::returnRandomNumber(2, mWidth - 2)]
                [GetRandomNumber::returnRandomNumber(2, mLength - 2)] = mPrize.getSign();

        mPlayGround[GetRandomNumber::returnRandomNumber(2, mWidth - 2)]
                [GetRandomNumber::returnRandomNumber(2, mLength - 2)] = KEY;

        mPlayGround[GetRandomNumber::returnRandomNumber(2, mWidth - 3)] // 3  щоб не закрило вихід і герой міг вийти
                [GetRandomNumber::returnRandomNumber(2, mLength - 2)] = BLACK_HOLE;

        mPlayGround[GetRandomNumber::returnRandomNumber(2, mWidth - 3)]
                [GetRandomNumber::returnRandomNumber(2, mLength - 2)] = ENEMY;


        mPlayGround[mWidth - 1][mLength/2] = EXIT;

        mPlayGround[mPlayer.getY()][mPlayer.getX()] = mPlayer.getCharacter();
    }

    int createPlayGround()
    {
        mWidth *= (mLevel);
        mLength *= (mLevel);

        if (mLength > 2 && mWidth  > 2)
        {
            mPlayGround = new char *[mWidth];
            mPlayGround[0] = new char[mWidth * mLength];

            for (int j(1); j != mWidth; ++j)
            {
                mPlayGround[j] = mPlayGround [j - 1] + mLength;
            }

            // i = y; j = x
            for (int i(0); i != mWidth; i++)
            {

                for (int j(0); j != mLength; j++)
                {
                    if (i == 0 || i == mWidth - 1)
                    {
                        mPlayGround[i][j] = WALL;

                    }
                    else if (j == 0 || j == mLength - 1)
                    {
                        mPlayGround[i][j] = WALL;
                    }
                    else
                    {
                        mPlayGround[i][j] = EMPTYSIGN;
                    }
                }
            }

            //MakeMaze Maze(m);

            //m = Maze.create(mPlayer.getX(), mPlayer.getY(), 3);

            setCharacters();
            return 1;
        }
        return  0;
    }

    int getPlayerPoints()
    {
        return mPlayer.getPoints();
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

    int play(const char direction)
    {

        move(direction);

        int decreaseMovePoints = -1 * static_cast<int>(mLevel);

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
                    [GetRandomNumber::returnRandomNumber(2, mLength - 2)]  = mPrize.getSign();

            mPlayGround[mPlayer.getY()][mPlayer.getX()] = EMPTYSIGN;


            mPlayer.setNewPosition(mPlayerMoveX, mPlayerMoveY);
        }
        else if (mPlayGround[mPlayerMoveY][mPlayerMoveX] == BLACK_HOLE)
        {
            mPlayerMoveX = GetRandomNumber::returnRandomNumber(2, mWidth - 2);
            mPlayerMoveY = GetRandomNumber::returnRandomNumber(2, mLength - 2);

            mPlayGround[mPlayer.getY()][mPlayer.getX()] = mPlayer.getCharacter();
        }
        else if (mPlayGround[mPlayerMoveY][mPlayerMoveX] == ENEMY)
        {
            mPlayGround[mPlayer.getY()][mPlayer.getX()] = EMPTYSIGN;
            mPlayer.setNewPosition(mPlayerMoveX, mPlayerMoveY);

            return 2;
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
        int  beginningWidthSize(10);
        int  beginningLengthSize(15);


        char direction;

        std::cout << "O - black hole - leap to unknown\n";
        std::cout << "$ - snake, it kills you\n";
        std::cout << "k - key to open exit\n";
        std::cout << "1, 2, 3 - treasure\n";
        std::cout << "# - exit\n";

        PlayGround A(beginningWidthSize, beginningLengthSize);



        while (true)
        {
               A.createPlayGround();
               int endOfGame{0};

                A.vizualize();

                while (true)
                {
                    std::cout << "Your points: " << A.getPlayerPoints() << '\n';
                    std::cout << "Enter direction to move : (u - up, d - down, l - left, r - right, q - quit)\n";
                    std::cout << "Your input: ";

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

                    if (endOfGame == 1)
                    {
                        std::cout << "Congratulations!\n";
                        std::cout << "You win!\n";
                        std::cout << "Your Points: " << A.getPlayerPoints() << '\n';
                        break;
                    }
                    else if (endOfGame == 2)
                    {
                        std::cout << "You lose!\n";
                        std::cout << "Your Points: " << A.getPlayerPoints() << '\n';
                        break;
                    }
                }

                if (endOfGame == 1 && direction != 'q')
                {
                    std::cout << "Next level? [Y/n]\n";
                    char response = 'n';
                    std::cout << "Your response: ";
                    std::cin >> response;
                    A.upgradeLevel();


                    if (response == 'N' || response == 'n')
                    {
                        break;
                    }
                }
                else if (endOfGame == 2 || direction == 'q')
                {
                    std::cout << "Play again? [Y/n]\n";
                    std::cout << "Your response: ";
                    char response = 'n';
                    std::cin >> response;


                    if (response == 'N' || response == 'n')
                    {
                        break;
                    }
                    else
                    {
                        A.startFromBeginning(beginningWidthSize, beginningLengthSize);
                    }

                }
             }
        }

    };


int main()
{
    UserInterface::userInterface();
    return 0;
}
