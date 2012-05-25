TESTS = t_bud t_budget t_report t_lb
lib_LTLIBRARIES = liblb.la
liblb_la_SOURCES =  lb.cpp lbrc.cpp report.cpp budget.cpp lb.h lbrc.h bud.h report.h budget.h
bin_PROGRAMS = lb 
noinst_PROGRAMS = t_bud t_budget t_report t_lb
lb_SOURCES = main.cpp
lb_LDADD = liblb.la
t_bud_SOURCES = t_bud.cpp ../test/tester.cpp ../test/tester.h bud.h
t_budget_SOURCES = t_budget.cpp ../test/tester.cpp ../test/tester.h budget.h
t_budget_LDADD = liblb.la
t_report_SOURCES = t_report.cpp ../test/tester.cpp ../test/tester.h report.h
t_report_LDADD = liblb.la
t_lb_SOURCES = t_lb.cpp ../test/tester.cpp ../test/tester.h lb.h bud.h
t_lb_LDADD = liblb.la






