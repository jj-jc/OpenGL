#ifndef COMMLOOP_H_
#define COMMLOOP_H_

#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include <tbb/atomic.h>
#include <tbb/mutex.h>
#include <tbb/concurrent_queue.h>
#include "tbb/concurrent_hash_map.h"

typedef unsigned int cs_t;

namespace torre
{

    class CommLoop     //: public tbb::task
    {

    private:
        int rtm;
        int site;

    public:
        typedef struct PanoramicPoint
        {
                int x;
                int y;
        } PanoramicPoint;

        typedef struct PanoramicColor
        {
                int r;
                int g;
                int b;
        } PanoramicColor;

        typedef struct TextProperty
        {
                uint id;
                std::string text;
        } TextProperty;

        typedef struct PanoramicEntityH
        {
            std::string id;
            std::string label;
            int type;
            int originX;
            int originY;
            int originZ;
            int height;
            int width;
            int contentSizePercent;
            std::string videoSource;
            PanoramicColor background;
            PanoramicColor foreground;
            float opacity;
            bool enabled;
            bool visible;
            bool textVisible;
    //                    bool blocked;
    //                    int idBlocked;
            long int windowZoom;
            std::vector<PanoramicPoint> points;
            std::vector<TextProperty> textProperties;
            bool isHotspot;
            int originalZ;
            long int linkedVideoWindow;
            int subtype;
            int stippled;
            std::string iconName;
            int fontSize;
            int fontFamily;
            bool filled;
            int lineWidht;

            virtual void printHelloWorld(){}
            virtual void attach() {}
            virtual void detach() {}
            virtual void update(double ts) {}
            virtual void renderImGui() {}
        //                    int idPTZ;
        } PanoramicEntityH;

        // std::map<cs_t, tbb::concurrent_hash_map<std::string, PanoramicEntityH>> panoramicEntitysMap;
        // tbb::concurrent_hash_map<std::string, PanoramicEntityH> panoramicEntitysCommon;
        PanoramicEntityH* m_panoramicEntityH_ptr;
        PanoramicEntityH m_panoramicEntityH;
        int test;

        void setTest(int value) {test = value;}
    };
}

#endif /* COMMMOUSES_H_ */