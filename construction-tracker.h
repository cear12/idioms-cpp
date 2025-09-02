#ifndef CONSTRUCTION_TRACKER_H
#define CONSTRUCTION_TRACKER_H


#include <QString>
#include <QDebug>


namespace construction_tracker
{
    //
    class Tracker
    {
    private:
        enum TrackerType { TrackNone, TrackA, TrackB, TrackCount };

    private:
        struct TrackA
        {
            TrackA( int i /* must have any param */ ) { /* nothing to do */ }

        } trackA_;

        struct TrackB
        {
            TrackB( QString s /* must have any param */ ) { throw "interrupted"; }

        } trackB_;

    public:
        Tracker( TrackerType tracker = TrackNone )
        try
            : trackA_( ( tracker = TrackA, 0         ) ) // Can throw std::exception
            , trackB_( ( tracker = TrackB, QString() ) ) // Can throw std::exception
        {
            Q_ASSERT( tracker == TrackB );        // ... constructor body ...
        }
        catch( ... )
        {
            qDebug() << "catched on step: " << tracker << " of " << TrackCount;
        }
    };

    //
    void test()
    {
        Tracker tracker;
    };
}


#endif // CONSTRUCTION_TRACKER_H
