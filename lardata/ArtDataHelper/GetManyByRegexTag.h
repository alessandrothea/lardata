/// @file  GetManyByRegexTag.h
/// @brief Utility to retrieve art data products whose input tags match a regex pattern.

#ifndef LARDATA_LARDATA_ARTDATAHELPER_GETMANYBYREGEXTAG_H
#define LARDATA_LARDATA_ARTDATAHELPER_GETMANYBYREGEXTAG_H

#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Selector.h"
#include "canvas/Persistency/Provenance/BranchDescription.h"
#include "canvas/Utilities/InputTag.h"

#include <regex>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

/// LArSoft-specific namespace
namespace lar {

  /// LArSoft utility namespace
  namespace util {

    /**
     * @brief  Retrieve all data products of type `T` whose input tags match a
     *         regex pattern.
     *
     * Each field of @p tag (label, instance, process) is interpreted as a
     * `std::regex` pattern.  An empty field acts as the wildcard `".*"` and
     * matches any value.  All three fields must match for a product to be
     * selected.
     *
     * @tparam T      The data-product type to retrieve (e.g. `recob::Hit`).
     * @param  e      The current art event.
     * @param  tag    An `art::InputTag` whose fields are treated as regex
     *                patterns rather than literal strings.
     * @return        A vector of valid `art::Handle<T>` for every matching
     *                product in the event.
     * @throws std::runtime_error  If no products of type `T` match the given
     *                             pattern.
     *
     * ### Example
     * @code
     * // Retrieve all recob::Hit collections whose label starts with "gauss".
     * auto handles = lar::util::getManyByRegexTag<recob::Hit>(
     *     event, art::InputTag{"gauss.*", "", ""});
     * @endcode
     */
    template<typename T>
    std::vector<art::Handle<T>>
    getManyByRegexTag(art::Event& e, const art::InputTag& tag)
    {
      std::regex instance_regex(!tag.instance().empty() ? tag.instance() : ".*");
      std::regex label_regex   (!tag.label()   .empty() ? tag.label()    : ".*");
      std::regex process_regex (!tag.process() .empty() ? tag.process()  : ".*");

      art::SelectorByFunction selector(
        [instance_regex, label_regex, process_regex](art::BranchDescription const& p) {
          return std::regex_match(p.inputTag().label(),    label_regex)
              & std::regex_match(p.inputTag().instance(), instance_regex)
              & std::regex_match(p.inputTag().process(),  process_regex);
        },
        "InputTag Regex Selector"
      );

      auto handles = e.getMany<T>(selector);
      if (handles.empty()) {
        throw std::runtime_error(
          "No " + std::string(typeid(T).name()) +
          " collections matching " + tag.instance() + "_" + tag.label());
      }
      return handles;
    }
  }   // namespace util
} // namespace lar

#endif // LARDATA_LARDATA_ARTDATAHELPER_GETMANYBYREGEXTAG_HH
