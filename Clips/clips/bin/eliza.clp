

(deffunction nonASCII (?chaine) "La chaîne commence par un caractère accentué"
	(bind ?car (sub-string 1 1 ?chaine))
	(or (eq ?car (sub-string 1 1 "é"))
		(eq ?car (sub-string 1 1 "è"))
		(eq ?car (sub-string 1 1 "ê"))
		(eq ?car (sub-string 1 1 "ë"))
		(eq ?car (sub-string 1 1 "à"))
		(eq ?car (sub-string 1 1 "â"))
		(eq ?car (sub-string 1 1 "ô"))
		(eq ?car (sub-string 1 1 "ù"))
		(eq ?car (sub-string 1 1 "ç"))))
		
(deffunction str-car (?chaine) "Fonction CAR sur les chaînes de caractères"
	(if (nonASCII ?chaine)
		then (return (sub-string 1 2 ?chaine))
		else (return (sub-string 1 1 ?chaine))))

(deffunction str-cdr (?chaine) "Fonction CDR sur les chaînes de caractères"
	(if (nonASCII ?chaine)
		then (return (sub-string 3 (str-length ?chaine) ?chaine))
		else (return (sub-string 2 (str-length ?chaine) ?chaine))))

(deffunction str-clean (?chaine) "Pour supprimer tous les accents d'une chaîne de caractères"
	(bind ?car (str-car ?chaine))
	(bind ?cdr (str-cdr ?chaine))
	(switch ?car
		(case "" then (return ""))
		(case "é" then (return (str-cat "e" (str-clean ?cdr))))
		(case "è" then (return (str-cat "e" (str-clean ?cdr))))
		(case "ê" then (return (str-cat "e" (str-clean ?cdr))))
		(case "ë" then (return (str-cat "e" (str-clean ?cdr))))
		(case "à" then (return (str-cat "a" (str-clean ?cdr))))
		(case "â" then (return (str-cat "a" (str-clean ?cdr))))
		(case "ù" then (return (str-cat "u" (str-clean ?cdr))))
		(case "ô" then (return (str-cat "o" (str-clean ?cdr))))
		(case "ç" then (return (str-cat "c" (str-clean ?cdr))))
		(default (return (str-cat ?car (str-clean ?cdr))))))




(deffacts faits-initiaux
	(output "Bonjour, pouvez-vous me donner des informations ?"))

(defrule bonjour
	?in <- (input bonjour)
	=>
	(retract ?in)
	(assert (output "qu'est ce qui vous tracasse ?")))

(defrule themes-q-f-n-htz
	?theme <- (theme ?)
	?in <- (input non $?)
	?q <- (question fermee)
	?f <-(frequence)	
	=>
	(retract ?f)
	(retract ?q)
	(retract ?theme)
	(retract ?in)
	(assert (output "C'est une expérience peu banale"))
)
	

(defrule regle-bonjour
	?fact <- (input $? beau temps $?)
	=>
	(retract ?fact)
	(assert(output "Après le beau temps, c'est la pluie !")))

(defrule au-revoir
	?fact <- (input $? au revoir $?)
	=>
	(printout t "au revoir, j'ai apprécié notre bavardage, revenez vite !" crlf)
	(retract ?fact))
(defrule info
	?fact <- (input $?deb est $?fin)
	=>
	(retract ?fact)
	(assert (relation $?deb est $?fin))
	(assert (output merci pour cette information)))
(defrule themes
	?fact <- (input j'ai ?v un|une|des|de|du ?a $?)
	=>
	(assert(theme ?a))
	(retract ?fact)
	(assert(output "Cela vous arrive-t-il souvent de faire des" ?a))
	(assert(question fermee))
	(assert(frequence)))
(defrule connaissance
	?fact <- (input $? connaissances)
	=>
	(retract ?fact)
	(assert (output "moi, je peux acquérir des connaissances en bavardant")))

(defrule ketoyes
	?in <- (input c'est quoi $?deb ?)
	?save <- (relation $?deb est $?fin)
	=>
	(assert (output $?deb est $?fin je crois))
	(retract ?in))
(defrule ketoyes2
	?in <- (input $? c'est quoi $?obj ?)
	=>
	(retract ?in)
	(assert (output je suis bien incapable de vous renseigner sur  $?obj )))

(defrule nimporte-koa
	?in <- (input $?)
	=>
	(retract ?in)
	(assert (output "pouvez-vous m’en dire plus ?")))
(defrule interaction
	?fact <-(output $?phrase)
	=>
	(printout t $?phrase crlf)
	(retract ?fact)
	
	(assert(input (explode$ (lowcase (str-clean (readline)))))))