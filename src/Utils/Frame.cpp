#include <future>
#include "Frame.hpp"
#include "ImageCreator.hpp"
#include "Part.hpp"

using namespace std;

Frame::Frame(const Context &context) :
        frameContext(context), numberOfParts(11), dimX(context.dimension[0]), dimY(context.dimension[1]) {
    auto partLength = static_cast<unsigned int>(floor(context.dimension[1] / (numberOfParts)));
    partLengthInPixels = partLength * context.dimension[0] * 3;

    for (unsigned int i = 0; i < numberOfParts; ++i) {
        parts_.push_back(createPart(i, partLength));
    }
    results.resize(dimX * dimY * 3);
}

Part Frame::createPart(unsigned partNr, unsigned partLength) {
    Context currentContext(frameContext);
    currentContext.window[0][0] = 0;
    currentContext.window[1][0] = currentContext.dimension[0];

    currentContext.window[0][1] = (partNr) * partLength;
    if (partNr == numberOfParts - 1) {
        currentContext.window[1][1] = currentContext.dimension[1];
    }
    else {
        currentContext.window[1][1] = (partNr + 1) * partLength;
    }

    return Part(currentContext, partNr);
}

void Frame::saveToImage(const Pixels &pixels, const int partNr, const int taskId) const {
    if (partNr + 1 != numberOfParts) {
        assert(partLengthInPixels == pixels.size());
        int end = partNr * partLengthInPixels + partLengthInPixels;
        int old = 0;
        for (int i = partNr * partLengthInPixels; i < end; ++i) {
            results[i] = pixels[old];
            ++old;
        }

    }
    else {
        assert(pixels.size() == dimX * dimY * 3 - partNr * partLengthInPixels);
        int old = 0;
        for (int i = partNr * partLengthInPixels; i < dimX * dimY * 3; ++i) {
            results[i] = pixels[old];
            ++old;
        }
    }

    if (partNr == parts_.size() - 1) {
        ImageCreator imgCreator;
        imgCreator.saveToImage(results, dimX, dimY, frameContext.frameId, taskId);
    }
}

void Frame::generateImage() const {
    std::vector<std::future<Pixels>> futures;
    auto start = std::chrono::steady_clock::now();
    for (const auto &part : parts_) {
        futures.push_back(std::async(std::launch::async, std::bind(&Part::calculatePart, part)));
    }


    //retrive and print the value stored in the future
    unsigned featureCount = 0;
    for (auto &e : futures) {
        Pixels resultsPixels = e.get();
        saveToImage(resultsPixels, featureCount, 1);
        featureCount++;
    }
    auto end = std::chrono::steady_clock::now();

    auto diff = end - start;
    std::cout << "Frame::Calculation duration: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
}