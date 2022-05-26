template <typename T>
class Score
{
    private:
        T score;
    public:
        T getScore() const { return score;}
        explicit Score(T score) : score{score} {}
        explicit Score(const Score<T>& s) : score{s.score} {}
        Score<T>& operator+=(T s) {this->score += s; return *this;}
};