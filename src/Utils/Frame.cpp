#include <future>
#include "Frame.hpp"
#include "ImageCreator.hpp"
#include "Part.hpp"

using namespace std;

Frame::Frame(const Context &context) :
        numberOfPixelColors_(3),
        numberOfParts_(11),
        dimensionX_(context.dimension[0]),
        dimensionY_(context.dimension[1]),
        partLength_(static_cast<unsigned int>(floor(dimensionY_ / (numberOfParts_)))),
        frameContext(context) {

    for (unsigned int i = 0; i < numberOfParts_; ++i) {
        parts_.push_back(createPart(i, partLength_));
    }
    results_.resize(dimensionX_ * dimensionY_ * numberOfPixelColors_);
}

Part Frame::createPart(unsigned partNr, unsigned partLength) {
    Context currentContext(frameContext);
    const auto partSize = (partNr) * partLength;
    currentContext.window[0][0] = 0;
    currentContext.window[1][0] = dimensionX_;

    currentContext.window[0][1] = partSize;
    currentContext.window[1][1] = std::min(dimensionY_, (partNr + 1) * partLength);

    return Part(currentContext, partNr);
}

void Frame::saveToImage(const Pixels &pixels, const int partNr, const int taskId) const {
    auto partLengthInPixels =
            dimensionX_ * numberOfPixelColors_ * partLength_;
    unsigned pixelOffset = partNr * partLengthInPixels;

    for (const auto &pixel : pixels) {
        results_[pixelOffset] = pixel;
        ++pixelOffset;
    }

    if (partNr == parts_.size() - 1) {
        ImageCreator imgCreator;
        imgCreator.saveToImage(results_, dimensionX_, dimensionY_, frameContext.frameId, taskId);
    }
}

void Frame::generateImage() const {
    std::vector<std::future<Pixels>> futures;
    auto start = std::chrono::steady_clock::now();

    std::transform(std::begin(parts_), std::end(parts_), std::back_inserter(futures),
            [](const std::remove_reference<decltype(parts_)>::type::value_type &part) {
                return std::async(std::launch::async, std::bind(&Part::calculatePart, part));
            });

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