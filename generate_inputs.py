import random

random.seed(42)


def gen_table_groups(total):
    num_groups = random.randint(2, min(4, total))
    caps = [random.randint(2, 8) for _ in range(num_groups)]
    counts = [1] * num_groups
    remaining = total - num_groups
    for i in range(num_groups - 1):
        if remaining == 0:
            break
        give = random.randint(0, remaining)
        counts[i] += give
        remaining -= give
    counts[-1] += remaining
    return list(zip(counts, caps))


def gen_request_fields(typ):
    size = random.randint(1, 10)
    price = random.randint(50, 2000)
    if typ in ('ODG', 'ODN'):
        return {'size': size, 'price': price,
                'seats': random.randint(1, 8),
                'dur': random.randint(10, 60),
                'share': random.choice(['Y', 'N'])}
    elif typ in ('OVC', 'OVG', 'OVN'):
        return {'size': size, 'price': price, 'dist': random.randint(500, 5000)}
    else:
        return {'size': size, 'price': price}


def action_to_line(a):
    if a['kind'] == 'X':
        return f"X {a['tq']} {a['id']}"
    typ = a['typ']
    f = a['fields']
    base = f"Q {typ} {a['tq']} {a['id']} {f['size']} {f['price']}"
    if typ in ('ODG', 'ODN'):
        return f"{base} {f['seats']} {f['dur']} {f['share']}"
    elif typ in ('OVC', 'OVG', 'OVN'):
        return f"{base} {f['dist']}"
    return base


def generate_file(filename, M, numCN, numCS, numScooters, numTables, TH,
                  cancel_frac, type_weights, max_tq):
    num_cancels_target = round(M * cancel_frac)
    num_requests = M - num_cancels_target

    types = [t for t, w in type_weights.items() if w > 0]
    weights = [type_weights[t] for t in types]

    req_tqs = sorted(random.randint(1, max_tq) for _ in range(num_requests))
    req_types = random.choices(types, weights=weights, k=num_requests)

    requests = []
    ovc_pool = []
    for oid, (tq, typ) in enumerate(zip(req_tqs, req_types), start=1):
        fields = gen_request_fields(typ)
        requests.append({'kind': 'Q', 'typ': typ, 'tq': tq, 'id': oid, 'fields': fields})
        if typ == 'OVC':
            ovc_pool.append((oid, tq))

    cancels = []
    pool_copy = list(ovc_pool)
    random.shuffle(pool_copy)
    for target_id, target_tq in pool_copy:
        if len(cancels) >= num_cancels_target:
            break
        cancels.append({'kind': 'X',
                        'tq': random.randint(target_tq, max_tq),
                        'id': target_id})

    all_actions = sorted(requests + cancels, key=lambda a: a['tq'])

    table_groups = gen_table_groups(numTables)

    lines = [
        f"{numCN} {numCS}",
        "3 5",
        f"{numScooters} 50",
        "5 20",
        f"{numTables}",
    ]
    for count, cap in table_groups:
        lines.append(f"{count} {cap}")
    lines.append(f"{TH}")
    lines.append(f"{len(all_actions)}")
    for a in all_actions:
        lines.append(action_to_line(a))

    with open(filename, 'w') as f:
        f.write('\n'.join(lines) + '\n')

    n_req = sum(1 for a in all_actions if a['kind'] == 'Q')
    n_can = sum(1 for a in all_actions if a['kind'] == 'X')
    n_ovc = sum(1 for a in all_actions if a.get('typ') == 'OVC')
    print(f"{filename}: {len(all_actions)} actions ({n_req} requests, {n_can} cancels), {n_ovc} OVC")


configs = [
    dict(filename='input1.txt', M=50, numCN=4, numCS=2, numScooters=3, numTables=5, TH=8,
         cancel_frac=0.24,
         type_weights={'OVC': 60, 'OVN': 15, 'OVG': 10, 'ODN': 10, 'OT': 5, 'ODG': 0},
         max_tq=20),
    dict(filename='input2.txt', M=100, numCN=5, numCS=3, numScooters=4, numTables=8, TH=8,
         cancel_frac=1/12,
         type_weights={'ODG': 17, 'ODN': 17, 'OT': 17, 'OVN': 17, 'OVC': 16, 'OVG': 16},
         max_tq=40),
    dict(filename='input3.txt', M=150, numCN=4, numCS=2, numScooters=5, numTables=10, TH=8,
         cancel_frac=1/12,
         type_weights={'ODG': 30, 'OVG': 30, 'ODN': 15, 'OVN': 10, 'OT': 10, 'OVC': 5},
         max_tq=60),
    dict(filename='input4.txt', M=200, numCN=6, numCS=4, numScooters=6, numTables=6, TH=8,
         cancel_frac=1/12,
         type_weights={'ODG': 25, 'ODN': 50, 'OT': 10, 'OVN': 8, 'OVG': 4, 'OVC': 3},
         max_tq=80),
    dict(filename='input5.txt', M=250, numCN=8, numCS=4, numScooters=4, numTables=12, TH=8,
         cancel_frac=1/12,
         type_weights={'OVN': 40, 'OVC': 25, 'OVG': 25, 'ODN': 5, 'OT': 3, 'ODG': 2},
         max_tq=100),
    dict(filename='input6.txt', M=300, numCN=8, numCS=5, numScooters=8, numTables=15, TH=10,
         cancel_frac=1/12,
         type_weights={'ODG': 12, 'ODN': 18, 'OT': 20, 'OVN': 22, 'OVC': 15, 'OVG': 13},
         max_tq=120),
]

for cfg in configs:
    generate_file(**cfg)
