DELETE FROM REPLACE_PATTERNS;

INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Код символьных посл', '&#([0-9]+);', '^\1^', 2, 1);
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Пояснения', '#([0-9\sА-ЯЁа-яёA-Za-z\.\-;:,"\!\?–—“”«»\(\)''’/\{\}\^]+)#', '<span class="note">\1</span>', 2, 2);					  
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Абзац', '\r\n', '', 2, 3);
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Искаженная форма', '([А-ЯЁа-яё'']+){([А-ЯЁа-яё'']+)\*}', '<distinct form="\1">\2</distinct>', 2, 3);
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Знаки препинания', '[,;:—]{1}', '/', 2, 3);
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Граница строки в стихе', '\$', '<br></br>', 2, 3);
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Заголовок 1 уровня', '@h([1-4])([\sА-ЯЁа-яё\.\-;:,"\!\?–—“”«»\{\}/]+)', '<p class="H\1">\2</p>', 2, 3);
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Неразборчиво', '\[нрзб\]', '<noindex>\0</noindex>', 2, 3);
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Ударение', '[''’]', '&#769;', 2, 4);					  
INSERT INTO REPLACE_PATTERNS (ID, NAME, REGEXP, PATTERN, USAGE_TYPE, PRIORITY)
                      VALUES (gen_id(GEN_REPLACE_PATTERNS_ID, 1), 'Декод символьных посл', '\^([0-9]+)\^', '&#\1;', 2, 5);