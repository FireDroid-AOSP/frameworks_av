/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef DRM_H_

#define DRM_H_

#include "SharedLibrary.h"

#include <media/IDrm.h>
#include <utils/threads.h>

namespace android {

struct DrmFactory;
struct DrmPlugin;

struct Drm : public BnDrm {
    Drm();
    virtual ~Drm();

    virtual status_t initCheck() const;

    virtual bool isCryptoSchemeSupported(const uint8_t uuid[16]);

    virtual status_t createPlugin(const uint8_t uuid[16]);

    virtual status_t destroyPlugin();

    virtual status_t openSession(Vector<uint8_t> &sessionId);

    virtual status_t closeSession(Vector<uint8_t> const &sessionId);

    virtual status_t
        getLicenseRequest(Vector<uint8_t> const &sessionId,
                          Vector<uint8_t> const &initData,
                          String8 const &mimeType, DrmPlugin::LicenseType licenseType,
                          KeyedVector<String8, String8> const &optionalParameters,
                          Vector<uint8_t> &request, String8 &defaultUrl);

    virtual status_t provideLicenseResponse(Vector<uint8_t> const &sessionId,
                                            Vector<uint8_t> const &response);

    virtual status_t removeLicense(Vector<uint8_t> const &sessionId);

    virtual status_t queryLicenseStatus(Vector<uint8_t> const &sessionId,
                                        KeyedVector<String8, String8> &infoMap) const;

    virtual status_t getProvisionRequest(Vector<uint8_t> &request,
                                         String8 &defaulUrl);

    virtual status_t provideProvisionResponse(Vector<uint8_t> const &response);

    virtual status_t getSecureStops(List<Vector<uint8_t> > &secureStops);

    virtual status_t releaseSecureStops(Vector<uint8_t> const &ssRelease);

    virtual status_t getPropertyString(String8 const &name, String8 &value ) const;
    virtual status_t getPropertyByteArray(String8 const &name,
                                          Vector<uint8_t> &value ) const;
    virtual status_t setPropertyString(String8 const &name, String8 const &value ) const;
    virtual status_t setPropertyByteArray(String8 const &name,
                                          Vector<uint8_t> const &value ) const;

private:
    mutable Mutex mLock;

    status_t mInitCheck;
    sp<SharedLibrary> mLibrary;
    DrmFactory *mFactory;
    DrmPlugin *mPlugin;

    static KeyedVector<Vector<uint8_t>, String8> mUUIDToLibraryPathMap;
    static KeyedVector<String8, wp<SharedLibrary> > mLibraryPathToOpenLibraryMap;
    static Mutex mMapLock;

    void findFactoryForScheme(const uint8_t uuid[16]);
    bool loadLibraryForScheme(const String8 &path, const uint8_t uuid[16]);
    void closeFactory();


    DISALLOW_EVIL_CONSTRUCTORS(Drm);
};

}  // namespace android

#endif  // CRYPTO_H_
