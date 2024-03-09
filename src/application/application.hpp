
namespace db
{

    class Application
    {

    public:
        virtual void run(int argc, char *argv[]) = 0;
    };

    class DefaultApplication : public Application
    {
    public:
        void run(int argc, char *argv[]) override;
    };

}