```python
#!/edatools/system/python/python-3.6.6/bin/python3

import argparse, os ,re

PROJ_HOME = os.getenv("PROJ_HOME")

file_list = {
"id":'{}/commit_id.txt'.format(PROJ_HOME),
"comp":'{}/tmp/execroot/sp/bazel-out/k8-fastbuild/bin/src/design/rtl/sim/comp_sp/comp.log'.format(PROJ_HOME),
"lint":'{}/bazel-out/k8-fastbuild/bin/src/meta/lint/sp_top_rtl_spyglass_dir/sg_results/spyglass/consolidated_reports/sp_top_lint_lint_rtl/spyglass.log'.format(PROJ_HOME),
"cdc":'{}/tmp/execroot/sp/bazel-out/k8-fastbuild/bin/src/meta/cdc/sp_top_cdc_spyglass_dir/sg_results/spyglass/consolidated_reports/sp_top_cdc_cdc_verify_struct/spyglass.log'.format(PROJ_HOME),
"syn":'{}/syn_home/Oneflow2/workspaces/sp_top_IP_SYN_test/reports/synthesis/sp_top.mapped.proc_qor.rpt'.format(PROJ_HOME),
}

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="QA_notes")
    parser.add_argument(
            "-c",
            "--command",
            default="comp",
            help="Specify command such as comp,lint,cdc,syn",
            )
    args = parser.parse_args()
    command = args.command
    
# latch data
   #commit_id 
    if os.path.exists(file_list.get('id')):
        print('update commit id')
        commit_id = []
        f = open(file_list.get('id'),'r',encoding='utf-8')
        lines = f.readlines()
        for line in lines:
            find_commit = re.findall(r'commit',line)
            if (find_commit):
                commit_id = re.findall(r'(?<=commit )\w+',line)
            break
        f.close()
    else:
        print('commit_id.txt does not exist')
    #comp
    if (command == 'comp'):
        if os.path.exists(file_list.get('comp')):
            print('update compile_nodes')
            comp_num = []
            f = open(file_list.get('comp'),'r',encoding='utf-8')
            lines = f.readlines()
            lastline = lines[-1]
            comp_num = re.findall(r'\b\d+\b',lastline)
            f.close()
        else:
            print('comp.log does not exist')
    #lint
    elif (command == 'lint'):
        if os.path.exists(file_list.get('lint')):
            print('update lint_nodes')
            f = open(file_list.get('lint'),'r',encoding='utf-8')
            lines = f.readlines()
            lint_num = []
            for line in lines:
                find_messages = re.findall(r'Reported Messages:',line)
                if (find_messages):
                    lint_num = re.findall(r'\b\d+\b',line)
            f.close()
        else:
            print('spyglass.log does not exist')
    #cdc
    elif (command == 'cdc'):
        if os.path.exists(file_list.get('cdc')):
            print('update cdc_notes')
            f = open(file_list.get('cdc'),'r')
            lines = f.readlines()
            cdc_num = []
            for line in lines:
                find_messages = re.findall(r'Reported Messages:',line)
                if (find_messages):
                    cdc_num = re.findall(r'\b\d+\b',line)
        else:
            print('spyglass.log does not exist')
    #syn
    elif (command == 'syn'):
        if os.path.exists(file_list.get('syn')):
            print('update synthesis_nodes')
            f = open(file_list.get('syn'),'r',encoding='utf-8')
            lines = f.readlines()
            copy_area = 0
            copy_timing = 0
            area_note = [] 
            timing_note = []
            for line in lines:                
                find_area = re.findall(r'^Area',line)
                end_area = re.findall(r'^Net Length',line)
                find_timing = re.findall(r'^Path Group',line)
                end_timing = re.findall(r'^Summary',line)                
                if (copy_area == 1):
                    find_ignore = re.findall(r'^-----',line)
                    if not (find_ignore):
                        area_name = re.findall(r'^(.*?)(?=:)',line)
                        area_value = re.findall(r'\b\d+\.?\d*\b',line)
                        area_name_str = ''.join(area_name)
                        area_value_str = ''.join(area_value)
                        new_line = "|"+ area_name_str + "|" + area_value_str + "|" + "\n"
                        area_note.append(new_line)
                if (copy_timing == 1):
                    find_ignore = re.findall(r'^-----',line)
                    if not (find_ignore):
                        timing = re.findall(r'\S+\b',line)
                        timing_str = '|'.join(timing)
                        new_line = "|"+ timing_str + "|" + "\n"
                        timing_note.append(new_line)
                if (find_area):
                    copy_area = 1
                elif (end_area):
                    copy_area = 0
                if (find_timing):
                    copy_timing = 1
                elif (end_timing):
                    copy_timing = 0
            f.close()
        else:
            print('sp_top.mapped.proc_qor.rpt does not exist')

    
#update data
    new_note = ""
    #compile
    if (command == 'comp'):
        if os.path.exists(file_list.get('comp')):
            f = open('{}/docs/design/SP_QA_Notes.md'.format(PROJ_HOME),'r',encoding='utf-8')
            get_name = 0
            for line in f:
                if (get_name == 0):
                    find_name = re.findall(r'# Compile',line)
                    if(find_name):
                        get_name = 1 
                else:
                    find_error = re.findall(r'- Error:',line)
                    find_warning = re.findall(r'- Warning:',line)
                    if(find_error):
                        line = re.sub(r'(?<=Error:)\d+',comp_num[0],line)
                    elif(find_warning):
                        line = re.sub(r'(?<=Warning:)\d+',comp_num[1],line)
                    find_lint = re.findall(r'# Lint',line)
                    if (find_lint):
                        get_name = 0  
                new_note += line
            f.close()
            f = open('{}/docs/design/SP_QA_Notes.md'.format(PROJ_HOME),'w+',encoding='utf-8')
            f.write(new_note)
            f.close()
    #lint
    elif (command == 'lint'):
        if os.path.exists(file_list.get('lint')):        
            f = open('{}/docs/design/SP_QA_Notes.md'.format(PROJ_HOME),'r',encoding='utf-8')
            get_name = 0
            for line in f:
                if (get_name == 0):
                    find_name = re.findall(r'^# Lint',line)
                    if(find_name):
                        get_name = 1 
                else:
                    find_error = re.findall(r'- Error:',line)
                    find_warning = re.findall(r'- Warning:',line)
                    find_commit_id = re.findall(r'- Commit ID:',line)
                    if(find_error):
                        line = re.sub(r'(?<=Error:)\d+',lint_num[1],line)
                    elif(find_warning):
                        line = re.sub(r'(?<=Warning:)\d+',lint_num[2],line)
                    elif(find_commit_id):
                        line = re.sub(r'(?<=Commit ID:)\w+',commit_id[0],line)
                    find_cdc = re.findall(r'# CDC',line)
                    if(find_cdc):
                        get_name = 0  
                new_note += line
            f.close()
            f = open('{}/docs/design/SP_QA_Notes.md'.format(PROJ_HOME),'w+',encoding='utf-8')
            f.write(new_note)
            f.close()
    #cdc
    elif (command == 'cdc'):
        if os.path.exists(file_list.get('cdc')):        
            f = open('{}/docs/design/SP_QA_Notes.md'.format(PROJ_HOME),'r')
            get_name = 0
            for line in f:
                if (get_name == 0):
                    find_name = re.findall(r'# CDC',line)
                    if(find_name):
                        get_name = 1 
                else:
                    find_error = re.findall(r'- Error:',line)
                    find_warning = re.findall(r'- Warning:',line)
                    find_commit_id = re.findall(r'- Commit ID:',line)
                    if(find_error):
                        line = re.sub(r'(?<=Error:)\d+',cdc_num[1],line)
                    elif(find_warning):
                        line = re.sub(r'(?<=Warning:)\d+',cdc_num[2],line)
                    elif(find_commit_id):
                        line = re.sub(r'(?<=Commit ID:)\w+',commit_id[0],line)
                    find_syn = re.findall(r'Synthesis',line)   
                    if (find_syn):
                        get_name = 0  
                new_note += line
            f.close()
            f = open('{}/docs/design/SP_QA_Notes.md'.format(PROJ_HOME),'w+')
            f.write(new_note)
            f.close()

    #syn
    elif (command == 'syn'):
        if os.path.exists(file_list.get('syn')):        
            f = open('{}/docs/design/SP_QA_Notes.md'.format(PROJ_HOME),'r',encoding='utf-8')
            get_name = 0
            update_area_len = 0
            update_timing_len = 0
            note_copy_area = 0
            note_copy_timing = 0
            for line in f:
                if (get_name == 0):
                    find_name = re.findall(r'Synthesis',line)
                    if(find_name):
                        get_name = 1 
                else:
                    note_find_area = re.findall(r'(?<=|)Value',line)
                    note_find_timing = re.findall(r'Path Group',line)
                    find_commit_id = re.findall(r'- Commit ID:',line)
                    if (find_commit_id):
                        line = re.sub(r'(?<=Commit ID:)\w+',commit_id[0],line)
                    if (note_copy_area == 1):
                        find_ignore = re.findall(r'------',line)
                        if not (find_ignore):
                            line = area_note[update_area_len]
                            update_area_len += 1
                    if (note_copy_timing == 1):
                        find_ignore = re.findall(r'------',line)
                        if not (find_ignore):
                            line = timing_note[update_timing_len]
                            update_timing_len += 1
                    if (note_find_area):
                        note_copy_area = 1
                    elif (update_area_len ==len(area_note)):
                        note_copy_area = 0
                    if (note_find_timing):
                        note_copy_timing = 1
                    elif (update_timing_len == len(timing_note)):
                        note_copy_timing = 0
                    if (update_area_len == len(area_note) and update_timing_len == len(timing_note) and find_commit_id):
                        get_name = 0  
                new_note += line
            f.close()
            f = open('{}/docs/design/SP_QA_Notes.md'.format(PROJ_HOME),'w+',encoding='utf-8')
            f.write(new_note)
            f.close()

```

