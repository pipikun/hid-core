#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import json
import re
import subprocess

class Convert:

    def __init__(self):
        self.raw = None
        self.raw_pc  = list()
        self.pc_list = list()

    def load_json(self, path):
        with open(path, 'r', encoding='utf8')as fp:
            self.raw = json.load(fp)

    def dump_json(self, path):
        with open(path,"w") as fp:
            json.dump(self.raw, fp)

    def __get_index(self, src, name):
        for n, f in enumerate(src):
            if f['file'] == name:
                return n
        return None

    def load_simpc(self, path):
        with open(path, 'r') as fp:
            line = fp.readlines()
            self.pc_list.append(line)
        return self.pc_list

    def load_objdump(self, path):
        with open(path, 'r') as fp:
            for line in fp.readlines():
                out  = re.match(r'\s+([0-9A-Fa-f]+):'
                                r'\s+.+?'
                                r'([0-9A-Fa-f]+)\s+', line)
                if out is None:
                    continue
                pc = out[1]
                if pc in self.raw_pc:
                    continue
                self.raw_pc.append(pc)
        return self.raw_pc

    def __tack_line(self, info):
        data = info.split(':')
        name = data[0].split('/')
        line = data[1].split(' ')[0]
        file_name = name[-2]+ '/' + name[-1]
        file_line = int(line)
        return file_name, file_line

    def pc2line(self, pc_list, exe_file, addr2line):
        pc_cnt    = dict()
        file_list = dict()
        line_list = dict()
        for pc in pc_list:
            if pc in pc_cnt:
                pc_cnt.update({pc:pc_cnt[pc]+1})
                continue
            info = subprocess.Popen(args   = [addr2line, '-e', exe_file , pc],
                                    shell  = False,
                                    stdout = subprocess.PIPE).stdout.read().decode('utf-8')
            if ('?' in info):
                continue
            pc_cnt.update({pc:1})
            name, line = self.__tack_line(info)
            file_list.update({pc:name})
            line_list.update({pc:line})
        return pc_cnt,line_list, file_list

    def dump_rawpc(self, path):
        with open(path, 'w') as fp:
            for pc in self.raw_pc:
                fp.write(hex(pc)+'\n')

    def __get_line(self, src, line_number):
        for n, line in enumerate(src.get('lines')):
            if line['line_number'] == line_number:
                if line['gcovr/noncode'] is True:
                    return n - 1
                else:
                    return n
        return None
    def __update_branche(self, src, cnt):
        branch = src.get('branches')
        if len(branch) == 0:
            return None
        for b in  branch:
            print(b)
        return src

    def __update_line(self, name, line, num):
        src = self.raw.get('files')
        n1 = self.__get_index(src, name)
        if n1 is None:
            return
        n2 = self.__get_line(src[n1], line)
        if n2 is None:
            return
        # update count
        self.raw.get('files')[n1]['lines'][n2].update({'count': num})
        # update branches
        self.__update_branche(self.raw.get('files')[n1]['lines'][n2], num)

    def update(self, name_list, line_list, pc_list):
        assert(len(name_list.keys()) == len(pc_list.keys()))
        for pc in pc_list:
            name = name_list[pc]
            num  = pc_list[pc]
            line = line_list[pc]
            self.__update_line(name, line, num)

    def review_json(self):
        for i,file_name in enumerate(self.raw['files']):
            for j, line in enumerate(file_name['lines']):
                cnt = line['count']
                en  = line['gcovr/noncode'] == False

if __name__ == "__main__":
    objdump_file = f'./build/hid-core.dump'
    exe_file  = f'./build/hid-core'
    addr2line = 'addr2line'

    conv = Convert()
    conv.load_json("./build/hid-core.json")
    conv.load_objdump(objdump_file)

    # conv.update('app/main.c', 16, 123)
    # conv.dump_rawpc('build/raw_pc.pc')

    pc, line, name = conv.pc2line(conv.raw_pc, exe_file, addr2line)
    conv.update(name, line, pc)
    conv.dump_json('build/hid-core.json')
