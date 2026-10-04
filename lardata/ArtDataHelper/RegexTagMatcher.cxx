#include "lardata/ArtDataHelper/RegexTagMatcher.h"

#include <regex>

namespace lar::util::detail {
  RegexTagMatcher::RegexTagMatcher(art::InputTag const& regexTag) : fRegexTag{regexTag} {}

  bool RegexTagMatcher::match(art::InputTag const& tag) const
  {

    std::regex instance_re(!fRegexTag.instance().empty() ? fRegexTag.instance() : ".*");
    std::regex label_re(!fRegexTag.label().empty() ? fRegexTag.label() : ".*");
    std::regex process_re(!fRegexTag.process().empty() ? fRegexTag.process() : ".*");

    return std::regex_match(tag.label(), label_re) &&
           std::regex_match(tag.instance(), instance_re) &&
           std::regex_match(tag.process(), process_re);
  }
}