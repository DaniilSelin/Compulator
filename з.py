#!/usr/bin/env python3
# generate_transition_table.py

import re
import pandas as pd

# --- Настройки ---
INPUT_XLSX = 'tp.xlsx'  # ваш Excel-файл
SHEET_NAME  = 0                       # имя листа или его индекс
OUTPUT_H    = 'transition_table.h'

def parse_cell(cell):
    """
    Возвращает два списка строк: output и action.
    'λ' или NaN → ([], [])
    символ '□' → пустая строка ""
    если в ячейке есть пробел, то всё после первого пробела идёт во второй список.
    """
    if pd.isna(cell) or str(cell).strip() == 'λ':
        return [], []

    s = str(cell).strip()
    # разбор полного текста (до первого пробела)
    head, *tail = s.split(None, 1)
    def into_list(txt):
        out = []
        for ch in txt:
            if ch == '□':
                out.append('')
            else:
                out.append(ch)
        return out

    out1 = into_list(head)
    out2 = into_list(tail[0]) if tail else [''] * len(out1)
    return out1, out2

# читаем Excel
df = pd.read_excel(INPUT_XLSX, sheet_name=SHEET_NAME, dtype=str)
# первая колонка — имена состояний
states = df.iloc[:, 0].tolist()
# остальные — заголовки с номерами лексем
tokens = [int(x) for x in df.columns[1:]]

with open(OUTPUT_H, 'w', encoding='utf-8') as f:
    f.write('#pragma once\n\n')
    f.write('#include <map>\n#include <vector>\n#include <string>\n\n')
    f.write('enum class State { ' + ', '.join(states) + " };\n\n")
    f.write('struct Transition {\n')
    f.write('    std::vector<std::string> output;\n')
    f.write('    std::vector<std::string> action;\n')
    f.write('};\n')
    f.write('using TransitionMap = std::map<int, Transition>;\n\n')
    f.write('struct Table {\n')
    f.write('    std::map<State, TransitionMap> data;\n')
    f.write('};\n\n')
    f.write('static const Table kTransitionTable = {\n')
    f.write('  {\n')
    for si, state in enumerate(states):
        f.write(f'    {{ State::{state}, {{\n')
        row = df.iloc[si, 1:].tolist()
        for tok, cell in zip(tokens, row):
            out1, out2 = parse_cell(cell)
            # подготовка C++-литералов
            if not out1 and not out2:
                lit1 = lit2 = '""'
            else:
                lit1 = ', '.join(f"\"{s}\"" for s in out1)
                lit2 = ', '.join(f"\"{s}\"" for s in out2)
            f.write(f'        {{ {tok}, {{ std::vector<std::string>{{{lit1}}}, std::vector<std::string>{{{lit2}}} }} }},\n')
        f.write(f'    }} }}{"," if si+1 < len(states) else ""}\n\n')
    f.write('  }\n')
    f.write('};\n')

print(f"Сгенерирован {OUTPUT_H}")
