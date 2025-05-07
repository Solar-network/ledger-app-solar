#include <string.h>
#include <stddef.h>

/**
 * @brief Host-side fallback implementation of BSD strlcat().
 *
 * @details Provides a compatible strlcat() implementation for test environments
 *          where the native function is unavailable (such as glibc < 2.38).
 *          This implementation:
 *            - Is used exclusively in test harnesses
 *            - Is never deployed to production devices
 *            - Preserves behavior compatibility with BSD strlcat()
 *            - Avoids modifications to audited production code
 *
 * @param[in,out] dst   Destination buffer (must be NULL-terminated or size=0)
 * @param[in]     src   Source string to append
 * @param[in]     size  Total size of dst buffer, including space for NUL
 *
 * @return Total length that would result from concatenation:
 *         strlen(dst) + strlen(src), regardless of buffer constraints
 *
 * @note Unlike strcat(), this function guarantees NULL-termination when
 *       size > 0, even if truncation occurs
 */
size_t strlcat(char *dst, const char *src, size_t size) {
    size_t dn = strnlen(dst, size);
    size_t sn = strlen(src);
    if (dn + 1 < size) {
        size_t cp = size - 1 - dn;
        if (cp > sn) {
            cp = sn;
        }
        memcpy(dst + dn, src, cp);
        dst[dn + cp] = '\0';
    } return dn + sn;
}
