(define (domain vampire)
    (:requirements :conditional-effects)
    (:predicates
        (light-on ?r)
        (slayer-is-alive)
        (slayer-is-in ?r)
        (vampire-is-alive)
        (vampire-is-in ?r)
        (fighting)
        ;
        ; static predicates
        (NEXT-ROOM ?r ?rn)
        (CONTAINS-GARLIC ?r)
    )

    (:action toggle-light
        :parameters (?anti-clockwise-neighbor ?room ?clockwise-neighbor)
        :precondition (and
            (NEXT-ROOM ?anti-clockwise-neighbor ?room)
            (NEXT-ROOM ?room ?clockwise-neighbor)
            (not (fighting))
        )
        :effect (and
            ;; Toggle lights
            (when
                (light-on ?room)
                (not (light-on ?room)))
            (when
                (not (light-on ?room))
                (light-on ?room))

            ;; light goes on, vampire moves
            (when
                (and (vampire-is-in ?room) (not (light-on ?room)))
                (and
                    (not (vampire-is-in ?room))

                    ;; both dark, prefers anti-clockwise
                    (when
                        (and (not (light-on ?anti-clockwise-neighbor)) (not (light-on ?clockwise-neighbor)))
                        (and
                            (vampire-is-in ?anti-clockwise-neighbor)

                            ;; if met slayer, fight
                            (when
                                (slayer-is-in ?anti-clockwise-neighbor)
                                (fighting))
                        ))

                    ;; one dark, prefers it
                    (when
                        (and (not (light-on ?anti-clockwise-neighbor)) (light-on ?clockwise-neighbor))
                        (and
                            (vampire-is-in ?anti-clockwise-neighbor)

                            ;; if met slayer, fight
                            (when
                                (slayer-is-in ?anti-clockwise-neighbor)
                                (fighting))
                        ))

                    (when
                        (and (light-on ?anti-clockwise-neighbor) (not (light-on ?clockwise-neighbor)))
                        (and
                            (vampire-is-in ?clockwise-neighbor)

                            ;; if met slayer, fight
                            (when
                                (slayer-is-in ?clockwise-neighbor)
                                (fighting))
                        ))

                    ;; no dark, prefers clockwise
                    (when
                        (and (light-on ?anti-clockwise-neighbor) (light-on ?clockwise-neighbor))
                        (and
                            (vampire-is-in ?clockwise-neighbor)

                            ;; if met slayer, fight
                            (when
                                (slayer-is-in ?clockwise-neighbor)
                                (fighting))
                        ))
                ))

            ;; light goes off, slayer moves
            (when
                (and (slayer-is-in ?room) (light-on ?room))
                (and
                    (not (slayer-is-in ?room))

                    ;; clockwise if light
                    (when
                        (light-on ?clockwise-neighbor)
                        (and
                            (slayer-is-in ?clockwise-neighbor)

                            ;; if met vampire, fight
                            (when
                                (vampire-is-in ?clockwise-neighbor)
                                (fighting))
                        ))

                    ;; else, anti
                    (when
                        (not (light-on ?clockwise-neighbor))
                        (and
                            (slayer-is-in ?anti-clockwise-neighbor)

                            ;; if met vampire, fight
                            (when
                                (vampire-is-in ?anti-clockwise-neighbor)
                                (fighting))
                        ))
                )
            )
        )
    )

    (:action watch-fight
        :parameters (?room)
        :precondition (and
            (slayer-is-in ?room)
            (slayer-is-alive)
            (vampire-is-in ?room)
            (vampire-is-alive)
            (fighting)
        )
        :effect (and
            ;; dark and no garlic, slayer dead, vampire alive
            (when
                (and (not (light-on ?room)) (not (CONTAINS-GARLIC ?room)))
                (and (not (slayer-is-alive)) (not (fighting))))

            ;; garlic or light, slayer alive, vampire dead
            (when
                (or (light-on ?room) (CONTAINS-GARLIC ?room))
                (and (not (vampire-is-alive)) (not (fighting))))
        )
    )
)