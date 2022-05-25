template <typename T>
class Score
{
    private:
        T score;
    public:
        T getScore() const { return score;}
        explicit Score(T score) : score{score} {}
};