#ifndef UPTANE_SECONDARYINTERFACE_H
#define UPTANE_SECONDARYINTERFACE_H

#include <string>

#include "libaktualizr/secondary_provider.h"
#include "libaktualizr/types.h"

class SecondaryInterface {
 public:
  SecondaryInterface() = default;
  virtual ~SecondaryInterface() = default;

  using Ptr = std::shared_ptr<SecondaryInterface>;

  virtual void init(std::shared_ptr<SecondaryProvider> secondary_provider_in) = 0;
  virtual std::string Type() const = 0;
  virtual Uptane::EcuSerial getSerial() const = 0;
  virtual Uptane::HardwareIdentifier getHwId() const = 0;
  virtual PublicKey getPublicKey() const = 0;

  virtual Uptane::Manifest getManifest() const = 0;
  virtual data::InstallationResult putMetadata(const Uptane::Target& target) = 0;
  virtual bool ping() const = 0;

  // return 0 during initialization and -1 for error.
  virtual int32_t getRootVersion(bool director) const = 0;
  virtual data::InstallationResult putRoot(const std::string& root, bool director) = 0;

  /**
   * Send firmware to a device. This operation should be both idempotent and
   * not commit to installing the new version. Where practical, the
   * implementation should pre-flight the installation and report errors now,
   * while the entire installation can be cleanly aborted.
   * Failures reported later (during SecondaryInterface::install()) can leave
   * a multi-ecu update partially applied.
   */
  virtual data::InstallationResult sendFirmware(const Uptane::Target& target,
                                                const api::FlowControlToken* flow_control) = 0;
  /**
   * Commit to installing an update.
   */
  virtual data::InstallationResult install(const Uptane::Target& target, const api::FlowControlToken* flow_control) = 0;

 protected:
  SecondaryInterface(const SecondaryInterface&) = default;
  SecondaryInterface(SecondaryInterface&&) = default;
  SecondaryInterface& operator=(const SecondaryInterface&) = default;
  SecondaryInterface& operator=(SecondaryInterface&&) = default;
};

#endif  // UPTANE_SECONDARYINTERFACE_H
