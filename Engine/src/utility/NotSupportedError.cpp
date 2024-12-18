#include "includes/utility/NotSupportedError.hpp"

NotSupportedError::NotSupportedError()
    : std::logic_error("Method not yet implemented.") {}
