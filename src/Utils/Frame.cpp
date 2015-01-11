#include "Frame.hpp"
#include "ImageCreator.hpp"
#include "Part.hpp"

using namespace std;

Frame::Frame(const Context &context) :
        frameContext(context), numberOfParts(8), dimX(context.dimension[0]), dimY(context.dimension[1]) {
    auto partLength = static_cast<unsigned int>(floor(context.dimension[1] / (numberOfParts)));
    partLengthInPixels = partLength * context.dimension[0] * 3;

    for (unsigned int i = 0; i < numberOfParts; ++i) {
        Context currentContext(context);
        currentContext.window[0][0] = 0;
        currentContext.window[1][0] = currentContext.dimension[0];

        currentContext.window[0][1] = (i) * partLength;
        if (i == numberOfParts - 1) {
            currentContext.window[1][1] = currentContext.dimension[1];
        }
        else {
            currentContext.window[1][1] = (i + 1) * partLength;
        }

        parts_.push_back(Part(currentContext, i));
    }
    results.resize(dimX * dimY * 3);
}


long Frame::saveToImage(std::shared_ptr<Pixels> pixels, const int partNr, const int taskId) {

    --leftParts;

    if (partNr + 1 != numberOfParts) {
        assert(partLengthInPixels == (*(pixels.get())).size());
        int end = partNr * partLengthInPixels + partLengthInPixels;
        int old = 0;
        for (int i = partNr * partLengthInPixels; i < end; ++i) {
            results[i] = (*(pixels.get()))[old];
            ++old;
        }

    }
    else {
        assert((*(pixels.get())).size() == dimX * dimY * 3 - partNr * partLengthInPixels);
        int old = 0;
        for (int i = partNr * partLengthInPixels; i < dimX * dimY * 3; ++i) {
            results[i] = (*(pixels.get()))[old];
            ++old;

        }
    }

    if (!leftParts) {
        ImageCreator imgCreator;
        imgCreator.saveToImage(results, dimX, dimY, frameContext.frameId, taskId);
        boost::posix_time::ptime mst2 = boost::posix_time::microsec_clock::universal_time();
        boost::posix_time::time_duration diff = mst2 - mst1;
        long int timeRender = diff.total_milliseconds();
        return timeRender;
    }
    else {
        return 0;
    }
}