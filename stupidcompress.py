def encode(a):
    out = []
    meta = []
    i = 0
    while True:
        if i == len(a):
            break

        end = i
        while end != len(a) and a[end] == a[i]:
            end += 1

        out.append(a[i])

        if end > i+3:
            meta.append((i, end))
            i = end
        else:
            i += 1

    return out, meta


def decode(a, meta):
    out = []
    i = 0
    for s in a:
        out.append(s)
        if len(meta) != 0:
            j, end = meta[0]
            if j == i:
                while end - 1 != i:
                    out.append(s)
                    i += 1
                meta.pop(0)
        i += 1

    return out
