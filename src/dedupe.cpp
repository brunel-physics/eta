#include "dedupe.hpp"

#include <mutex>
#include <unordered_set>
#include <utility>
#include <vector>

#include <boost/functional/hash.hpp>

#include <ROOT/RDataFrame.hxx>
#include <ROOT/RVec.hxx>

namespace
{
bool seen(const unsigned run, const ULong64_t event)
{
    static std::mutex mtx;
    static std::unordered_set<std::pair<unsigned, ULong64_t>, boost::hash<std::pair<unsigned, ULong64_t>>> seen_events{};

    std::scoped_lock lock{mtx};
    return seen_events.emplace(run, event).second;
}
}

void dedupe(int argc, char* argv[])
{
    // No faster (and perhaps even slower) to multi thread.
    // Bottleneck as only one thread can access the map at a time
    // ROOT::EnableImplicitMT();

    const std::vector<std::string> files{"/scratch/data/tZqSkimsRun2017/mumuRun2017B_eta/*.root",
                                         "/scratch/data/tZqSkimsRun2017/muRun2017B_eta/*.root"};
    ROOT::RDataFrame d{"Events", files};

    auto d_dup = d.Filter(seen, {"run", "event"}, "Event deduplicator");

    // TODO: Trying to save the tree currently causes a crash, figure out why
    // foo.Snapshot("Events", "deduplicated.root");

    auto allCutsReport{d.Report()};
    const auto cut{allCutsReport->At("Event deduplicator")};
    std::cout << "Found " << cut.GetAll() - cut.GetPass() << " duplicated events" << std::endl;
}