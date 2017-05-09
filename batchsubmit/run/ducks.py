import sys
sys.path.insert(0,"/home/users/phchang/cmstas/NtupleTools/AutoTwopler")
import run
import params as p
import mybaby as mybaby

# make instructions
instructions = []

# make instructions by class of sample
samples_types = ["backgrounds"]
for stype in samples_types:
    for ds in mybaby.d_ds2name[stype].keys():
        instructions.append({"executable": mybaby.executable, "package": mybaby.package, "analysis": "%s"%mybaby.analysis, "dataset": ds, "baby_tag": mybaby.tag, "type": "BABY", "extra": [-1, "output_tree.root"]})

# set the "params.py" in the AutoTwopler with my settings
p.dataset_to_shortname   = mybaby.dataset_to_shortname
p.dashboard_name         = mybaby.dashboard_name
p.sweepRoot_scripts      = mybaby.sweepRoot_scripts
p.merging_scripts        = mybaby.merging_scripts
p.baby_merged_dir        = mybaby.baby_merged_dir
p.merge_babies_on_condor = mybaby.merge_babies_on_condor
p.exit_when_done         = True

run.main(instructions=instructions, params=p)

